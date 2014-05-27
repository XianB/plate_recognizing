

void Sobel_dir( IplImage* img, IplImage* dst)
{
	int dx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
	int dy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

	int step = img->widthStep/sizeof(uchar);
	uchar* data = (uchar *)img->imageData;
	uchar* data_dst = (uchar *)dst->imageData;
	
	int s;
	for (int i=1; i < img->height-2; i++)
		for (int j=1; j < img->width-2; j++)
		{
			// apply kernel in X direction
			int sum_x=0;
			for(int m=-1; m<=1; m++)
				for(int n=-1; n<=1; n++)
				{
					s=data[(i+m)*step+j+n]; // get the (i,j) pixel value
					sum_x+=s*dx[m+1][n+1];
				}
			// apply kernel in Y direction
			int sum_y=0;
			for(int m=-1; m<=1; m++)
				for(int n=-1; n<=1; n++)
				{
					s=data[(i+m)*step+j+n]; // get the (i,j) pixel value
					sum_y+=s*dy[m+1][n+1];
				}
			int sum=abs(sum_x)+abs(sum_y);
			if (sum>255)
				sum=255;
			data_dst[i*step+j]=sum; // set the (i,j) pixel value
		}
}
