void Rgb2Hsv(float R, float G, float B, float& H, float& S, float&V)
{
// r,g,b values are from 0 to 1
// h = [0,360], s = [0,1], v = [0,1]
// if s == 0, then h = -1 (undefined)

float min, max, delta,tmp;
tmp = min(R, G);
min = min( tmp, B );
tmp = max( R, G);
max = max(tmp, B );
V = max; // v

delta = max - min;

if( max != 0 )
S = delta / max; // s
else
{
// r = g = b = 0 // s = 0, v is undefined
S = 0;
H = UNDEFINEDCOLOR;
return;
}
if( R == max )
H = ( G - B ) / delta; // between yellow & magenta
else if( G == max )
H = 2 + ( B - R ) / delta; // between cyan & yellow
else
H = 4 + ( R - G ) / delta; // between magenta & cyan

H *= 60; // degrees
if( H < 0 )
H += 360;
}
