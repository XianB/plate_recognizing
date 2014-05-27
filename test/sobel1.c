void Sobel_ind_fast( IplImage* img, IplImage* dst)
{
		CvScalar s;
			for (int i=1; i < img->height-2; i++)
					for (int j=1; j < img->width-2; j++)
							{
											// apply kernel in X and Y directions
											int sum_x=0;
														int sum_y=0;
																	for(int m=-1; m<=1; m++)
																					for(int n=-1; n<=1; n++)
																									{
																															s=cvGet2D(img,i+m,j+n); // get the (i,j) pixel value
																																				sum_x+=(int)s.val[0]*dx[m+1][n+1];
																																									sum_y+=(int)s.val[0]*dy[m+1][n+1];
																																													}
																				int sum=abs(sum_x)+abs(sum_y);
																							s.val[0]=(sum>255)?255:sum;
																										cvSet2D(dst,i,j,s); // set the (i,j) pixel value
																												}
}
