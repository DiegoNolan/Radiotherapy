#include "parsedata.h"

void parseOrganData(Sparse_Matrix<IloInt> & vox, vector<std::set<unsigned> > & organSets,
   VoxelMapping & voxelmap, vector<string> fnames, string vox_name)
{
   vector<Sparse_Matrix<IloInt> > voxs(fnames.size());
   string prefix = "../data/";
   // parse each file
	for(unsigned int it=0;it<fnames.size();++it)
	{
      // read file into memory
		cout << "------ Organ " << fnames[it] << " ------" << endl;
		char * buffer;

		string fname = prefix + fnames[it];
		FILE * infile;

		infile = fopen(fname.c_str(), "rb");
		if(infile == NULL){
			printf("Error opening file\n");
			std::cin.ignore();
			return;
		}

		unsigned long fileSize;
		size_t result;

		fseek(infile, 0, SEEK_END);
		fileSize = ftell(infile);
		rewind(infile);

		// allocate memory
		buffer = (char*) malloc( sizeof(char)*fileSize);

		// copy file to buffer
		result = fread(buffer, 1, fileSize, infile);
		if( result != fileSize){
			printf("Reading file error\n");
		}
		fclose(infile);

		printf("%d bytes read into memory\n", fileSize ); // done reading file into memory

		// get number of voxels
		unsigned int voxelCount = numVoxels(buffer, result);
		cout << "Number of voxels " << voxelCount << endl;

		// get beamlet counts
		vector<vec2> beamletCounts = getBeamletCounts(buffer, result);
		unsigned int totalBeamlets = getTotalBeamlets(beamletCounts);

		// get the dose matrix
		voxs[it] = getVoxMatrix(buffer, result, beamletCounts);
		cout << "Number of non-zeros is " << voxs[it].a.size() << endl;

		// get the set of the organ
		std::set<unsigned> organSet = getOrganSet(buffer, result);
		cout << "Size of organ is " << organSet.size() << " voxels" << endl;

      // get the voxel mapping
      if(it==0){
         voxelmap = getVoxelMapping(buffer, result);
         cout << endl;
         voxelmap.print();
         cout << endl;
      }

		free(buffer);

		// write the organ data
		writeSet(organSet, fname + string(".set"));
		organSets[it].insert(organSet.begin(), organSet.end());
		organSet.clear();

		// get the amount of beamlets that aren't even used
		vector<bool> js(voxs[it].n, false);
		// check if some beamlets do nothing
		for(unsigned int i=0;i<voxs[it].ja.size();++i)
		{
			js[ voxs[it].ja[i] ] = true;
		}

		unsigned int count =0;
		for(unsigned int i=0;i<js.size();++i)
		{
			if(js[i]){
				++count;
			}
		}

		cout << "Total of " << count << " unused j's " << endl;

	}

	// check that all voxs are the same
	for(unsigned int it=1;it<voxs.size();++it)
	{
		if(voxs[0] != voxs[it]){
			cout << "Error: These files don't produce the same vox matrix" << endl;
		}
	}
   // assigned the first Sparse_Matrix to the argument because we know they are all the same now
   vox = voxs[0];
   
	// Get more stats about the data
	// check to see if there are any rows that don't have non-zero values
	std::set<unsigned int> noBeamlets;
	for(unsigned int i=0;i<voxs[0].m;++i){
		if(!voxs[0].rowHasElems(i)){
			//cout << "voxel " << i << " has no beamlets that effect it" << endl;
			noBeamlets.insert(i);
		}
	}

	vector<std::set<unsigned int> > setIntersections(choose(organSets.size(), 2));
	// get the intersection between all sets
	unsigned int count = 0;
	for(unsigned int i=0;i<organSets.size();++i)
	{
		for(unsigned int j=i+1;j<organSets.size();++j)
		{
			std::set_intersection(organSets[i].begin(),
				organSets[i].end(), organSets[j].begin(), organSets[j].end(),
				std::inserter(setIntersections[count], setIntersections[count].end()));
			cout << "Intersection of " << fnames[i] << " and " << fnames[j] <<
				" has the size of " << setIntersections[count].size() << endl;
         // print the voxel of the first intersection
			if(setIntersections[count].size() != 0){
				cout << "Index of first intersecting element is " <<
               *setIntersections[count].begin() << endl;
			}
		}
	}

	cout << noBeamlets.size() << " out of " << voxs[0].m << " voxels receive no dose from any beamlet " << (double(noBeamlets.size())/double(voxs[0].m))*100. << "%" << endl;

	// get intersection of beamlets that don't get any dose and the organ sets
	vector<std::set<unsigned int> > noBeamIntersects(organSets.size());
	for(unsigned int i=0;i<noBeamIntersects.size();++i)
	{
		std::set_intersection(noBeamlets.begin(), noBeamlets.end(),
			organSets[i].begin(), organSets[i].end(),
			std::inserter( noBeamIntersects[i], noBeamIntersects[i].end()));
		cout << "The intersection voxels that receive no dose and organ " << fnames[i] <<
			" has the size " << noBeamIntersects[i].size() << endl;
	}

	// get the sum of all organ voxels
	unsigned int totalOrganVoxels = 0;
	for(unsigned int i=0;i<organSets.size();++i)
	{
		totalOrganVoxels += organSets[i].size();
	}

	cout << "There are " << totalOrganVoxels << " voxels in organs, assuming no intersections "
		" and that makes up " << (double(totalOrganVoxels)/double(voxs[0].m))*100. << "%" << endl;

	voxs[0].write(prefix +  vox_name + ".vox");

   // write the voxelmapping
   voxelmap.write(prefix + vox_name + ".voxmap");
}

bool isNum(char & ch)
{
	if(ch >= '0' && ch <= '9')
	{
		return true;
	}

	return false;
}

bool isDataLine(char & firstchar)
{
	if( isNum(firstchar) || firstchar == ' ' || firstchar == '-')
	{
		return true;
	}

	return false;
}

unsigned int numVoxels(char * buffer, unsigned long bufferSize)
{
	bool inData = false;
	unsigned int voxelCount = 0;
	for(unsigned long i=0;i<bufferSize;++i)
	{
		if(!inData)
		{
			if(buffer[i] == '\n')
			{
				if(isDataLine(buffer[i+1]))
				{
					inData = true;
				}
				i += MIN_LINE;
			}
		}
		else
		{
			if(buffer[i] == '\n')
			{
				++voxelCount;
				if(!isDataLine(buffer[i+1]))
				{
					return voxelCount;
				}
				i+=MIN_LINE;
			}
		}
	}

	// should never reach here
	return 0;
}

vector<vec2> getBeamletCounts(char * buffer, unsigned long bufferSize)
{
	vector<vec2> rXrY;

	for(unsigned long i=0;i<bufferSize;++i)
	{
		if(buffer[i] == '\n')
		{
			if(buffer[++i] == 'n')
			{
				rXrY.resize(rXrY.size()+1);
				rXrY[rXrY.size()-1].rx = 0;
				rXrY[rXrY.size()-1].ry = 0;

				while(!isNum(buffer[i]))
				{
					++i;
				}
				while(isNum(buffer[i]))
				{
					rXrY[rXrY.size()-1].rx *= 10;
					rXrY[rXrY.size()-1].rx += buffer[i]-48;
					++i;
				}

				while(!isNum(buffer[i]))
				{
					++i;
				}
				while(isNum(buffer[i]))
				{
					rXrY[rXrY.size()-1].ry *= 10;
					rXrY[rXrY.size()-1].ry += buffer[i]-48;
					++i;
				}
			}
		}
	}

	return rXrY;
}

unsigned int getTotalBeamlets(const vector<vec2>& beamletCounts)
{
	unsigned int totalBeamlets = 0;
	for(unsigned int i=0;i<beamletCounts.size();++i)
	{
		totalBeamlets += beamletCounts[i].rx*beamletCounts[i].ry;
	}
	
	cout << "Total beamlets is " << totalBeamlets << endl;
	return totalBeamlets;
}

Sparse_Matrix<IloInt> getVoxMatrix(char * buffer, unsigned long bufferSize,
	vector<vec2> beamletCounts)
{
	bool inData = false;
	short dose, beam_ind;
	beam_ind = -1; // start at negative one because the first beam is zero
	SparseMatrix vox;
	vox.a.resize(vox.a.size()+RESIZE_AMOUNT);
	vox.i.resize(vox.i.size()+RESIZE_AMOUNT);
	vox.j.resize(vox.j.size()+RESIZE_AMOUNT);
	unsigned int voxel_ind=0;
	unsigned int max;
	unsigned int num_non_zeros = 0;
	unsigned int j=0;

	for(unsigned long i=0;i<bufferSize;++i)
	{
		if(buffer[i] == '\n')
		{
			if(isDataLine(buffer[i+1]))
			{
				dose = 0;
				inData = true;
				i+=23;

				dose += buffer[i]-48;
				dose *= 10;
				dose += buffer[i+2]-48;
				dose *= 10;
				dose += buffer[i+3]-48;

				if(dose != 0)
				{
					bool negative = false;
					unsigned int p = i+12;
					short rx, ry;
					rx = ry = 0;
					// get rx
					while(buffer[p] != ' ')
					{
						if(buffer[p] == '-')
						{
							negative = true;
						}else if(isNum(buffer[p]))
						{
							rx *= 10;
							rx += buffer[p] - 48;
						}
						++p;
					}
					if(negative){
						rx *= -1;
					}

					negative = false;

					// get ry
					while(buffer[p] != ' ' && buffer[p] != '\n')
					{
						if(buffer[p] == '-')
						{
							negative = true;
						}else if(isNum(buffer[p]))
						{
							ry *= 10;
							ry += buffer[p] - 48;
						}
						++p;
					}
					if(negative){
						ry *= -1;
					}

					// beamlet has to be valid
					if(rx >= 0 && ry >= 0){
						if(num_non_zeros >= vox.a.size())
						{
							vox.a.resize(vox.a.size()+RESIZE_AMOUNT);
							vox.i.resize(vox.i.size()+RESIZE_AMOUNT);
							vox.j.resize(vox.j.size()+RESIZE_AMOUNT);
						}
				
						vox.a[num_non_zeros] = dose;
						vox.i[num_non_zeros] = voxel_ind;

                  // map the voxel to and index
						j= 0;
						for(short k=0;k<beam_ind;++k)
						{
							j += beamletCounts[k].rx*beamletCounts[k].ry;
						}

						j += beamletCounts[beam_ind].rx*ry + rx;

						vox.j[num_non_zeros] = j;

						++num_non_zeros;
					}
				}

				i+=MIN_LINE;
				++ voxel_ind;
				max = voxel_ind;
			}  // isDataLine
			else 
			{
				voxel_ind = 0;
				if(buffer[i+1] == 'B'){
					++beam_ind;
				}

				i+= MIN_LINE;
			}
		}
	}

	// clip vector sizes
	vox.a.resize(num_non_zeros);
	vox.i.resize(num_non_zeros);
	vox.j.resize(num_non_zeros);

	vox.m = max;
	vox.n = 0;
	for(unsigned int i=0;i<beamletCounts.size();++i)
	{
		vox.n += beamletCounts[i].rx*beamletCounts[i].ry;
	}

	// sort before converting to Sparse_Matrix
	vox.sort();

	Sparse_Matrix<IloInt> yale;
	yale.m = vox.m;
	yale.n = vox.n;
	yale.a.resize(vox.a.size());
	for(unsigned int i=0;i<vox.a.size();++i)
	{
		yale.a[i] = IloInt(vox.a[i]);
	}

	yale.ja = vox.j;

	yale.ia.resize(vox.m+1);

	j=0;
	for(unsigned int i=0;i<yale.ia.size();++i)
	{
		while(vox.i[j] < i){
			++j;
			if(j>=vox.i.size()){
				break;
			}
		}
		yale.ia[i] = j;

		if(j >= vox.i.size())
		{
			for(;i<yale.ia.size();++i){
				yale.ia[i] = j;
			}
			break;
		}
	}

	return yale;
}

set<unsigned> getOrganSet(char * buffer, unsigned int bufferSize)
{
	bool inData = false;
	vector<unsigned int> organSet(RESIZE_AMOUNT);

	unsigned int set_ind = 0;
	unsigned int voxel_ind =0;
	for(unsigned int i=0;i<bufferSize;++i)
	{
		if(buffer[i] == '\n')
		{
			if(isDataLine(buffer[i+1]))
			{
				inData = true;
				i+=31;
				if(buffer[i] == '1'){
					if(set_ind >= organSet.size()){
						organSet.resize(organSet.size()+RESIZE_AMOUNT);
					}
					organSet[set_ind] = voxel_ind;

					++set_ind;
				}
				++voxel_ind;
			}
			else
			{
				if(inData)
				{
					// clip organ set
					organSet.resize(set_ind);
					return std::set<unsigned>(organSet.begin(), organSet.end());
				}
			}
			
		}
	}

   cout << "Error: reached the end of file while reading set data" << endl;
	// should never reach here
	return std::set<unsigned>();
}

VoxelMapping getVoxelMapping(char * buffer, unsigned bufferSize)
{
   bool first=false;
   bool xchanged, ychanged, zchanged;
   xchanged = ychanged = zchanged = false;
   float xi, yi, zi, xs, ys, zs;
   unsigned xc, yc, zc;
   xc = yc = zc = 0;
   for(unsigned i=0;i<bufferSize;++i)
   {
      if(buffer[i] == '\n') // just hit new line
      {
         // check if next line is a dataline
         if(isDataLine(buffer[i+1]))
         {
            if(!first){
               std::string str(&buffer[i+1], XYZ_LENGTH);
               std::stringstream ss;
               ss << str;
               ss >> xi;
               ss >> yi;
               ss >> zi;
               first = true;
            }
            else if(!xchanged)
            {
               ++xc;
               ++yc;
               ++zc;
               std::string str(&buffer[i+1], XYZ_LENGTH);
               std::stringstream ss;
               ss << str;
               float xt;
               ss >> xt;

               if(!isEqual(xt, xi,FLT_TOL)){
                  xchanged = true;
                  xs = xt-xi;
               }
            }
            else if(!ychanged)
            {
               ++yc;
               ++zc;
               std::string str(&buffer[i+1], XYZ_LENGTH);
               std::stringstream ss;
               ss << str;
               float xt, yt;
               ss >> xt;
               ss >> yt;

               if(!isEqual(yt, yi,FLT_TOL)){
                  ychanged = true;
                  ys = yt-yi;
               }
            }
            else if(!zchanged)
            {
               ++zc;
               std::string str(&buffer[i+1], XYZ_LENGTH);
               std::stringstream ss;
               ss << str;
               float xt, yt, zt;
               ss >> xt;
               ss >> yt;
               ss >> zt;

               if(!isEqual(zt, zi,FLT_TOL)){
                  zchanged = true;
                  zs = zt-zi;
               }
            }
            else
            {
               break;
            }
         }
      } // is new line
   }

   return VoxelMapping(xs, ys, zs, xi, yi, zi, xc, yc, zc);
}


bool isEqual(float one, float two, float tol)
{
   if(two < one-tol || two > one+tol){
      return false;
   }

   return true;
}

bool fileExists(const std::string& filename)
{
   fstream file;
   file.open(filename.c_str(), ios_base::out | ios_base::in); // does not create file
   if( file.is_open())
   {
      file.close();
      return true;
   }

   cout << filename << " does not exist " << endl;
   return false;
}

bool allFiles(string vox_name, vector<string> organ_names)
{
   if( !fileExists("../data/" + vox_name + ".vox")){
      return false;
   }

   if( !fileExists("../data/" + vox_name + ".voxmap")){
      return false;
   }

   for(unsigned i=0;i<organ_names.size();++i)
   {
      if( !fileExists("../data/" + organ_names[i] + ".set") ){
         return false;
      }
   }

   return true;
}
