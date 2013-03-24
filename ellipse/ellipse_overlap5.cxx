#include <mex.h>
#include <math.h>

//
// [twout,faarea,fadr,fabna]=ellipse_overlap5(ell2,ell1,iscaling);
//
// twout - amount of overlap in some sense
// faarea - actual #pixels in the area of overlap
// fadr - resolution
// fabna - #"restricted-pixels" in the area of overlap
// ell<i> - name says it all fucker
// iscaling - amt of scaling before computing the overlap

void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
	// must have single double array input.
	if (nrhs != 4) { printf("in nrhs != 4\n"); return; }
	if (mxGetClassID(prhs[0]) != mxDOUBLE_CLASS) { printf("input must be a double array\n"); return; }
	if (mxGetClassID(prhs[1]) != mxDOUBLE_CLASS) { printf("input must be a double array\n"); return; }
  
	// must have single output.
	// if (nlhs != 4) { printf("out nlhs != 4\n"); return; }
  
	// get size of x.
	int const num_dims1 = mxGetNumberOfDimensions(prhs[0]);
	int const *dims1 = mxGetDimensions(prhs[0]);
	int const num_dims2 = mxGetNumberOfDimensions(prhs[1]);
	int const *dims2 = mxGetDimensions(prhs[1]);
	int const num_dims3 = mxGetNumberOfDimensions(prhs[2]);
	int const *dims3 = mxGetDimensions(prhs[2]);
	if(dims1[0]<9 || dims2[0]<9 || dims1[0]!=dims2[0] || dims3[0]!=1){ 
		printf("\n");
		if (dims1[0]<9) 			printf(" dims1[0] == %d <9 ",dims1[0]);
		if (dims2[0]<9) 			printf(" dims2[0] == %d <9 ",dims2[0]);
		if (dims1[0]!=dims2[0]) 	printf(" dims1[0]!=dims2[0] ");
		if (dims3[0]!=1) 			printf(" dims3[0]!=1 ");
		printf("dims != 9\n"); return;
	}
  
	// create new array of the same size as x.

	int *odim = new int[2];
	odim[0]=dims2[1];	odim[1]=dims1[1];
	int *odim_bounding_rect=new int[3];
	odim_bounding_rect[0]=dims2[1];	odim_bounding_rect[1]=dims1[1];	odim_bounding_rect[2]=4;
	
	plhs [0] = mxCreateNumericArray(2, odim, mxDOUBLE_CLASS, mxREAL);
	plhs [1] = mxCreateNumericArray(2, odim, mxDOUBLE_CLASS, mxREAL);
	plhs [2] = mxCreateNumericArray(2, odim, mxDOUBLE_CLASS, mxREAL);
	plhs [3] = mxCreateNumericArray(2, odim, mxDOUBLE_CLASS, mxREAL);
	plhs [4] = mxCreateNumericArray(3, odim_bounding_rect, mxDOUBLE_CLASS, mxREAL);

  
	// get pointers to beginning of x and y.
	double const *feat1 		= (double *)mxGetData(prhs[0]);
	double const *feat2 		= (double *)mxGetData(prhs[1]);
	double const *flag 			= (double *)mxGetData(prhs[2]);
	double const *dpResolution 	= (double *)mxGetData(prhs[3]);
	double       *mover_out 	= (double *)mxGetData(plhs[0]);	// twout
	double       *parea_out 	= (double *)mxGetData(plhs[1]);	// width-step
	double       *pdr_out 		= (double *)mxGetData(plhs[2]);	// width-step
	double       *pbna_out 		= (double *)mxGetData(plhs[3]);	// #pxls in the rectangle
	double       *prect_bounding= (double *)mxGetData(plhs[4]);	// #pxls in the rectangle


	float *feat1a = new float[9];
	float *feat2a = new float[9];

	//neo-h int common_part=(int)flag[0];
	double dScalingFactor=flag[0]; //neo-h

	float max_dist,fac,dist,dx,dy,bna,bua,descd,ov;
	for(int j=0,f1=0;j<dims1[1];j++,f1+=dims1[0]) {
		max_dist=sqrt(feat1[f1+5]*feat1[f1+6]);
		// fac: multiplicative factor
		// since desc is computed over 3*sigma, fac=3 during desc evaluation
		//neo-h if(common_part)	fac=30/max_dist;
		//neo-h else 			fac=3;
		fac=dScalingFactor;	// neo-h
	  
		max_dist=max_dist*8;
		fac=1.0/(fac*fac);
		feat1a[2]=fac*feat1[f1+2]; // a
		feat1a[3]=fac*feat1[f1+3]; // b
		feat1a[4]=fac*feat1[f1+4]; // c
		// 7: sqrt(c/(ac-b^2))
		// 8: sqrt(a/(ac-b^2))
		feat1a[7] = sqrt(feat1a[4]/(feat1a[2]*feat1a[4] - feat1a[3]*feat1a[3]));
		feat1a[8] = sqrt(feat1a[2]/(feat1a[2]*feat1a[4] - feat1a[3]*feat1a[3]));
		
		for (int i=0,f2=0; i<dims2[1]; i++,f2+=dims1[0]) {
			//compute shift error between ellipses
			dx=feat2[f2  ]-feat1[f1  ];
			dy=feat2[f2+1]-feat1[f1+1];
			dist=sqrt(dx*dx+dy*dy);

			if(dist<max_dist) {
				feat2a[2]=fac*feat2[f2+2]; //a
				feat2a[3]=fac*feat2[f2+3]; //b
				feat2a[4]=fac*feat2[f2+4]; //c
				feat2a[7] = sqrt(feat2a[4]/(feat2a[2]*feat2a[4] - feat2a[3]*feat2a[3]));
				feat2a[8] = sqrt(feat2a[2]/(feat2a[2]*feat2a[4] - feat2a[3]*feat2a[3]));

				//find the largest eigenvalue
				float maxx=ceil((feat1a[7]>(dx+feat2a[7]))?feat1a[7]:(dx+feat2a[7]));
				float minx=floor((-feat1a[7]<(dx-feat2a[7]))?(-feat1a[7]):(dx-feat2a[7]));
				float maxy=ceil((feat1a[8]>(dy+feat2a[8]))?feat1a[8]:(dy+feat2a[8]));
				float miny=floor((-feat1a[8]<(dy-feat2a[8]))?(-feat1a[8]):(dy-feat2a[8]));

				prect_bounding[(j*dims2[1]+i)*4+0]=minx;
				prect_bounding[(j*dims2[1]+i)*4+1]=maxx;
				prect_bounding[(j*dims2[1]+i)*4+2]=miny;
				prect_bounding[(j*dims2[1]+i)*4+3]=maxy;				

				//printf("x:%.f %.f y:%.f %.f",minx,maxx,miny,maxy);				
				float mina=(maxx-minx)<(maxy-miny)?(maxx-minx):(maxy-miny);
				float dr=mina/(*dpResolution); // resolution, in some sense
				bua=0;bna=0;int t1=0,t2=0;
				//compute the area
				for(float rx=minx;rx<=maxx;rx+=dr) {
					float rx2=rx-dx;t1++;
					for(float ry=miny;ry<=maxy;ry+=dr){
						float ry2=ry-dy;
						//compute the distance from the ellipse center
						float a=feat1a[2]*rx *rx  + 2*feat1a[3]*rx *ry  + feat1a[4]*ry *ry;
						float b=feat2a[2]*rx2*rx2 + 2*feat2a[3]*rx2*ry2 + feat2a[4]*ry2*ry2;
						//compute the area
						if(a<1 && b<1)bna++;
						if(a<1 || b<1)bua++;
					}
				}
				ov=100.0*(1-bna/bua);
				mover_out[j*dims2[1]+i]=ov;
				// bna - #pixels inside both the ellipses
				// dr  - resolution
				// dr*dr is needed coz dr*dr pixels are considered as one pixel
				parea_out[j*dims2[1]+i]=bna*dr*dr; 
				pdr_out[j*dims2[1]+i]=dr;
				pbna_out[j*dims2[1]+i]=bna;

				//printf("overlap %f  \n",over_out[j*dims2[1]+i]);return;
			}
			else {
				mover_out[j*dims2[1]+i]=100.0;
			}
		}
	}
  
	delete []odim;
	delete []feat1a;
	delete []feat2a;
}
