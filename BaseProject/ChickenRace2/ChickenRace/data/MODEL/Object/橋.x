xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 168;
 552.05035;0.71790;157.71207;,
 552.05035;0.71790;-156.91385;,
 552.05035;0.97280;-156.91385;,
 552.05035;0.97280;157.71207;,
 331.23029;-0.32100;-156.91385;,
 220.82007;-0.65160;-156.91385;,
 220.82007;15.07540;-156.91385;,
 331.23029;15.39410;-156.91385;,
 -441.64014;0.11850;157.71207;,
 -552.05035;0.71790;157.71207;,
 -552.05035;0.71790;-156.91385;,
 -441.64014;0.11850;-156.91385;,
 441.64008;15.85840;-156.91385;,
 331.23029;15.39410;-156.91385;,
 331.23029;15.39410;157.71207;,
 441.64008;15.85840;157.71207;,
 331.23029;15.39410;-156.91385;,
 220.82007;15.07540;-156.91385;,
 220.82007;15.07540;157.71207;,
 331.23029;15.39410;157.71207;,
 220.82007;15.07540;-156.91385;,
 110.41003;14.96620;-156.91385;,
 110.41003;14.96620;157.71207;,
 220.82007;15.07540;157.71207;,
 110.41003;14.96620;-156.91385;,
 0.00000;14.96620;-156.91385;,
 0.00000;14.96620;157.71207;,
 110.41003;14.96620;157.71207;,
 0.00000;14.96620;-156.91385;,
 -110.41003;14.96620;-156.91385;,
 -110.41003;14.96620;157.71207;,
 0.00000;14.96620;157.71207;,
 -220.82007;15.07540;-156.91385;,
 -331.23029;15.39410;-156.91385;,
 -331.23029;15.39410;157.71207;,
 -220.82007;15.07540;157.71207;,
 -331.23029;15.39410;-156.91385;,
 -441.64014;15.85840;-156.91385;,
 -441.64014;15.85840;157.71207;,
 -331.23029;15.39410;157.71207;,
 -441.64014;15.85840;-156.91385;,
 -552.05035;16.44360;-156.91385;,
 -552.05035;16.44360;157.71207;,
 -441.64014;15.85840;157.71207;,
 552.05035;0.71790;-156.91385;,
 441.64008;0.11850;-156.91385;,
 441.64008;15.85840;-156.91385;,
 552.05035;16.44360;-156.91385;,
 441.64008;0.11850;-156.91385;,
 331.23029;-0.32100;-156.91385;,
 331.23029;15.39410;-156.91385;,
 441.64008;15.85840;-156.91385;,
 220.82007;-0.65160;-156.91385;,
 110.41003;-0.76190;-156.91385;,
 110.41003;14.96620;-156.91385;,
 220.82007;15.07540;-156.91385;,
 110.41003;-0.76190;-156.91385;,
 0.00000;-0.76190;-156.91385;,
 0.00000;14.96620;-156.91385;,
 110.41003;14.96620;-156.91385;,
 0.00000;-0.76190;-156.91385;,
 -110.41003;-0.76190;-156.91385;,
 -110.41003;14.96620;-156.91385;,
 0.00000;14.96620;-156.91385;,
 -110.41003;-0.76190;-156.91385;,
 -220.82007;-0.65160;-156.91385;,
 -220.82007;15.07540;-156.91385;,
 -110.41003;14.96620;-156.91385;,
 -220.82007;-0.65160;-156.91385;,
 -331.23029;-0.32100;-156.91385;,
 -331.23029;15.39410;-156.91385;,
 -220.82007;15.07540;-156.91385;,
 -331.23029;-0.32100;-156.91385;,
 -441.64014;0.11850;-156.91385;,
 -441.64014;15.85840;-156.91385;,
 -331.23029;15.39410;-156.91385;,
 -441.64014;0.11850;-156.91385;,
 -552.05035;0.71790;-156.91385;,
 -552.05035;16.44360;-156.91385;,
 -441.64014;15.85840;-156.91385;,
 552.05035;16.44360;-156.91385;,
 441.64008;15.85840;-156.91385;,
 441.64008;15.85840;157.71207;,
 552.05035;16.44360;157.71207;,
 552.05035;16.44360;157.71207;,
 441.64008;15.85840;157.71207;,
 441.64008;0.11850;157.71207;,
 552.05035;0.71790;157.71207;,
 441.64008;15.85840;157.71207;,
 331.23029;15.39410;157.71207;,
 331.23029;-0.32100;157.71207;,
 441.64008;0.11850;157.71207;,
 331.23029;15.39410;157.71207;,
 220.82007;15.07540;157.71207;,
 220.82007;-0.65160;157.71207;,
 331.23029;-0.32100;157.71207;,
 220.82007;15.07540;157.71207;,
 110.41003;14.96620;157.71207;,
 110.41003;-0.76190;157.71207;,
 220.82007;-0.65160;157.71207;,
 110.41003;14.96620;157.71207;,
 0.00000;14.96620;157.71207;,
 0.00000;-0.76190;157.71207;,
 110.41003;-0.76190;157.71207;,
 0.00000;14.96620;157.71207;,
 -110.41003;14.96620;157.71207;,
 -110.41003;-0.76190;157.71207;,
 0.00000;-0.76190;157.71207;,
 -110.41003;14.96620;157.71207;,
 -220.82007;15.07540;157.71207;,
 -220.82007;-0.65160;157.71207;,
 -110.41003;-0.76190;157.71207;,
 -220.82007;15.07540;157.71207;,
 -331.23029;15.39410;157.71207;,
 -331.23029;-0.32100;157.71207;,
 -220.82007;-0.65160;157.71207;,
 -331.23029;15.39410;157.71207;,
 -441.64014;15.85840;157.71207;,
 -441.64014;0.11850;157.71207;,
 -331.23029;-0.32100;157.71207;,
 -441.64014;15.85840;157.71207;,
 -552.05035;16.44360;157.71207;,
 -552.05035;0.71790;157.71207;,
 -441.64014;0.11850;157.71207;,
 552.05035;0.71790;157.71207;,
 441.64008;0.11850;157.71207;,
 441.64008;0.11850;-156.91385;,
 552.05035;0.71790;-156.91385;,
 441.64008;0.11850;157.71207;,
 331.23029;-0.32100;157.71207;,
 331.23029;-0.32100;-156.91385;,
 441.64008;0.11850;-156.91385;,
 331.23029;-0.32100;157.71207;,
 220.82007;-0.65160;157.71207;,
 220.82007;-0.65160;-156.91385;,
 331.23029;-0.32100;-156.91385;,
 220.82007;-0.65160;157.71207;,
 110.41003;-0.76190;157.71207;,
 110.41003;-0.76190;-156.91385;,
 220.82007;-0.65160;-156.91385;,
 110.41003;-0.76190;157.71207;,
 0.00000;-0.76190;157.71207;,
 0.00000;-0.76190;-156.91385;,
 110.41003;-0.76190;-156.91385;,
 0.00000;-0.76190;157.71207;,
 -110.41003;-0.76190;157.71207;,
 -110.41003;-0.76190;-156.91385;,
 0.00000;-0.76190;-156.91385;,
 -110.41003;-0.76190;157.71207;,
 -220.82007;-0.65160;157.71207;,
 -220.82007;-0.65160;-156.91385;,
 -110.41003;-0.76190;-156.91385;,
 -220.82007;-0.65160;157.71207;,
 -331.23029;-0.32100;157.71207;,
 -331.23029;-0.32100;-156.91385;,
 -220.82007;-0.65160;-156.91385;,
 -331.23029;-0.32100;157.71207;,
 -441.64014;0.11850;157.71207;,
 -441.64014;0.11850;-156.91385;,
 -331.23029;-0.32100;-156.91385;,
 -552.05035;0.71790;-156.91385;,
 -552.05035;0.71790;157.71207;,
 -552.05035;16.44360;157.71207;,
 -552.05035;16.44360;-156.91385;,
 -110.41003;14.96620;-156.91385;,
 -220.82007;15.07540;-156.91385;,
 -220.82007;15.07540;157.71207;,
 -110.41003;14.96620;157.71207;;
 
 42;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;164,165,166,167;;
 
 MeshMaterialList {
  1;
  42;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "��.jpg";
   }
  }
 }
 MeshNormals {
  26;
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  -0.005429;-0.999985;0.000000;,
  -0.004705;-0.999989;0.000000;,
  -0.004753;0.999989;0.000000;,
  -0.003546;0.999994;0.000000;,
  -0.001938;0.999998;0.000000;,
  -0.000495;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000495;1.000000;0.000000;,
  0.001938;0.999998;0.000000;,
  0.003546;0.999994;0.000000;,
  0.004753;0.999989;0.000000;,
  0.005300;0.999986;0.000000;,
  -0.005300;0.999986;0.000000;,
  0.000000;0.000000;1.000000;,
  0.005429;-0.999985;-0.000000;,
  0.004705;-0.999989;-0.000000;,
  0.003487;-0.999994;-0.000000;,
  0.001997;-0.999998;-0.000000;,
  0.000500;-1.000000;-0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.000500;-1.000000;0.000000;,
  -0.001997;-0.999998;0.000000;,
  -0.003487;-0.999994;0.000000;,
  -1.000000;0.000000;0.000000;;
  42;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;3,2,2,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;10,11,11,10;,
  4;11,12,12,11;,
  4;12,13,13,12;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;14,4,4,14;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;16,17,17,16;,
  4;17,18,18,17;,
  4;18,19,19,18;,
  4;19,20,20,19;,
  4;20,21,21,20;,
  4;21,22,22,21;,
  4;22,23,23,22;,
  4;23,24,24,23;,
  4;24,3,3,24;,
  4;25,25,25,25;,
  4;9,10,10,9;;
 }
 MeshTextureCoords {
  168;
  0.992300;-0.002550;,
  0.002550;0.001510;,
  0.001240;1.000470;,
  0.999450;0.993650;,
  0.004660;0.005900;,
  0.005540;0.994800;,
  0.994490;0.988170;,
  0.993610;-0.000740;,
  -0.000250;0.000160;,
  0.000000;0.998660;,
  0.987740;0.996740;,
  0.987480;-0.001770;,
  0.002390;0.002580;,
  -0.000810;1.000540;,
  0.999630;0.997750;,
  0.999980;0.005590;,
  0.011470;0.002110;,
  0.011480;1.000050;,
  0.992990;0.999980;,
  0.992980;0.002040;,
  0.007350;0.000890;,
  0.007360;0.999630;,
  0.993260;0.999580;,
  0.993250;0.000850;,
  0.001990;0.004970;,
  0.000590;1.000280;,
  0.999150;0.993860;,
  0.999620;0.008450;,
  0.003590;0.002330;,
  0.003800;1.001250;,
  0.994540;0.999730;,
  0.994330;0.000810;,
  0.000720;0.001390;,
  0.000800;1.000370;,
  0.997460;0.999750;,
  0.997380;0.000770;,
  0.000870;0.003380;,
  0.001330;1.000160;,
  0.998390;0.996720;,
  0.997930;-0.000060;,
  0.000600;0.002040;,
  0.000910;0.999150;,
  0.999640;0.997960;,
  0.998810;-0.001010;,
  -0.005030;0.007630;,
  0.009210;0.997440;,
  1.002210;0.983150;,
  0.987970;-0.006650;,
  0.012090;-0.001500;,
  0.010220;0.992440;,
  1.001310;0.994290;,
  1.003180;0.000360;,
  0.002300;-0.000670;,
  0.003020;0.996190;,
  0.990020;0.990760;,
  0.989290;-0.006100;,
  -0.008340;0.011330;,
  0.008900;1.000090;,
  1.003750;0.982740;,
  0.986500;-0.006030;,
  0.003520;0.004330;,
  0.003980;0.991260;,
  0.998960;0.987750;,
  0.998490;0.000820;,
  0.002980;0.003190;,
  0.003670;0.994080;,
  0.992660;0.988890;,
  0.991960;-0.002000;,
  0.000170;0.003970;,
  0.000090;0.999040;,
  0.995760;0.999670;,
  0.995850;0.004590;,
  0.003210;0.001550;,
  0.003190;0.989530;,
  0.999150;0.989660;,
  0.999170;0.001680;,
  -0.001460;0.001340;,
  -0.000940;0.993760;,
  0.992780;0.989870;,
  0.992260;-0.002560;,
  0.001410;0.001870;,
  0.001320;0.991880;,
  0.995560;0.992620;,
  0.995660;0.002610;,
  -0.000930;-0.003040;,
  -0.000880;0.994820;,
  0.987590;0.994440;,
  0.987540;-0.003430;,
  -0.002200;0.004300;,
  -0.001570;0.990100;,
  0.991120;0.985400;,
  0.990500;-0.000400;,
  0.000520;0.008990;,
  0.001680;0.997520;,
  0.984710;0.988800;,
  0.983540;0.000270;,
  0.002220;0.006150;,
  0.002510;0.994170;,
  0.995680;0.992030;,
  0.995390;0.004010;,
  0.001620;-0.001090;,
  0.002190;0.995450;,
  0.993090;0.991160;,
  0.992520;-0.005380;,
  0.002170;0.001420;,
  0.001650;0.993820;,
  0.990130;0.997680;,
  0.990640;0.005280;,
  0.012930;0.001900;,
  0.013550;0.996430;,
  0.994680;0.991820;,
  0.994060;-0.002710;,
  0.010720;0.009690;,
  0.011220;0.996010;,
  0.997840;0.992250;,
  0.997340;0.005930;,
  0.011430;0.002270;,
  0.011420;0.996810;,
  0.987110;0.996930;,
  0.987130;0.002390;,
  0.003510;0.006400;,
  0.003870;0.998200;,
  1.001460;0.995540;,
  1.001100;0.003740;,
  -0.000260;-0.000700;,
  -0.000000;0.997800;,
  0.987730;0.995880;,
  0.987480;-0.002620;,
  -0.000270;-0.001570;,
  -0.000010;0.996930;,
  0.987730;0.995010;,
  0.987470;-0.003490;,
  -0.000260;-0.000430;,
  -0.000000;0.998070;,
  0.987730;0.996150;,
  0.987480;-0.002350;,
  -0.000250;0.000720;,
  0.000010;0.999220;,
  0.987740;0.997300;,
  0.987490;-0.001210;,
  -0.000250;-0.000150;,
  0.000000;0.998350;,
  0.987740;0.996430;,
  0.987480;-0.002070;,
  -0.000260;-0.001020;,
  -0.000010;0.997480;,
  0.987730;0.995560;,
  0.987470;-0.002940;,
  -0.000260;-0.000890;,
  -0.000000;0.997620;,
  0.987730;0.995700;,
  0.987470;-0.002800;,
  -0.000250;0.000260;,
  0.000000;0.998760;,
  0.987740;0.996840;,
  0.987480;-0.001660;,
  -0.000250;0.000400;,
  0.000000;0.998900;,
  0.987740;0.996980;,
  0.987480;-0.001530;,
  0.004900;-0.004360;,
  0.001570;0.993040;,
  0.985940;1.001670;,
  1.003520;0.004180;,
  0.001800;-0.001370;,
  0.001430;0.996010;,
  0.995240;0.998810;,
  0.995610;0.001430;;
 }
}
