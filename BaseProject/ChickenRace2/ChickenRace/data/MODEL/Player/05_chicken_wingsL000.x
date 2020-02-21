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
 28;
 6.92056;0.05276;7.14328;,
 4.66548;-0.21148;6.49660;,
 6.87180;1.46644;3.48788;,
 3.74860;1.21084;2.30576;,
 3.56884;0.25864;5.84616;,
 3.67112;1.22228;-1.11120;,
 6.70276;1.61872;-0.32436;,
 3.52448;1.60984;-2.57276;,
 6.49712;2.04556;-1.71056;,
 3.10512;2.21048;-0.92016;,
 6.49668;2.35444;-0.22692;,
 3.91264;2.63968;3.11776;,
 2.17500;1.78840;3.18120;,
 3.24712;0.48420;6.00248;,
 4.34196;0.33844;6.77136;,
 0.24560;-0.21168;2.29120;,
 0.42696;0.32836;0.34676;,
 0.36488;-0.05296;-1.12960;,
 0.31648;0.24084;-1.92928;,
 0.17504;0.39504;-1.06700;,
 1.40588;1.75540;1.63708;,
 0.73508;0.34316;3.19480;,
 2.28864;0.43040;4.93116;,
 0.01952;0.30300;2.39500;,
 0.13388;0.91596;0.57080;,
 6.32828;2.57844;3.81592;,
 4.93548;1.28372;5.61980;,
 6.70564;0.67068;7.43648;;
 
 40;
 3;0,1,2;,
 3;2,1,3;,
 3;1,4,3;,
 4;2,3,5,6;,
 4;6,5,7,8;,
 4;8,7,9,10;,
 3;10,9,11;,
 3;12,13,14;,
 4;4,15,16,3;,
 4;3,16,17,5;,
 4;5,17,18,7;,
 4;7,18,19,9;,
 3;9,19,20;,
 3;12,21,22;,
 3;13,22,4;,
 3;4,22,15;,
 3;22,21,15;,
 3;21,23,15;,
 4;16,15,23,24;,
 3;12,22,13;,
 3;24,23,20;,
 3;20,23,12;,
 3;23,21,12;,
 4;25,26,14,27;,
 3;25,11,26;,
 3;10,11,25;,
 3;9,12,11;,
 3;9,20,12;,
 3;19,24,20;,
 4;11,12,14,26;,
 4;27,14,1,0;,
 4;1,14,13,4;,
 3;24,19,16;,
 3;16,19,17;,
 3;19,18,17;,
 3;10,25,8;,
 3;8,25,6;,
 3;25,27,2;,
 3;27,0,2;,
 3;2,6,25;;
 
 MeshMaterialList {
  1;
  40;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  50;
  0.179920;-0.951756;-0.248573;,
  0.208627;-0.911218;-0.355185;,
  0.163882;-0.973279;-0.160841;,
  0.209596;-0.932830;-0.293084;,
  0.000450;0.989445;-0.144907;,
  -0.108432;0.936005;0.334869;,
  -0.225098;0.835497;0.501273;,
  0.157238;-0.974906;-0.157589;,
  -0.283100;-0.935663;0.210687;,
  0.059317;-0.941230;-0.332517;,
  0.247058;-0.959887;-0.132587;,
  0.273177;-0.913623;-0.301111;,
  -0.280519;0.949064;-0.143478;,
  -0.186210;0.964402;0.187764;,
  -0.470557;0.796908;0.378832;,
  -0.258139;0.824188;0.504062;,
  -0.425292;0.749104;0.507907;,
  0.342896;-0.937593;-0.057798;,
  0.367777;-0.903406;-0.220449;,
  0.329232;-0.938622;-0.102934;,
  0.335535;-0.890327;-0.307790;,
  -0.442395;0.855811;-0.268093;,
  -0.480045;0.877105;0.015600;,
  -0.606197;0.608772;0.511783;,
  -0.606193;0.608777;0.511782;,
  -0.868348;-0.434949;0.238307;,
  -0.900797;-0.433398;-0.027024;,
  -0.226662;0.825629;0.516682;,
  -0.017949;0.960571;-0.277456;,
  -0.249647;0.918520;-0.306591;,
  -0.467289;0.824755;-0.318464;,
  -0.443526;-0.783728;0.434804;,
  -0.526086;-0.664970;0.530140;,
  -0.739460;-0.516515;0.431754;,
  -0.666704;-0.417423;0.617465;,
  -0.904903;-0.424561;0.029995;,
  -0.522889;0.851142;-0.046300;,
  -0.630327;0.698579;0.338636;,
  -0.177478;-0.495478;0.850296;,
  -0.347719;-0.636952;0.688029;,
  -0.903325;-0.424178;0.063849;,
  -0.918854;-0.393491;0.029529;,
  -0.921670;-0.378901;-0.083424;,
  0.963821;-0.260897;0.054602;,
  0.964655;0.260884;0.037153;,
  0.996821;0.076258;0.023101;,
  0.905266;0.424524;-0.016526;,
  0.931504;0.346975;0.109124;,
  0.922400;0.380631;0.065566;,
  0.953453;0.285303;0.097619;;
  40;
  3;1,9,0;,
  3;0,9,7;,
  3;9,8,7;,
  4;0,7,10,2;,
  4;2,10,11,3;,
  4;28,29,12,4;,
  3;4,12,13;,
  3;14,16,15;,
  4;8,18,17,7;,
  4;7,17,19,10;,
  4;10,19,20,11;,
  4;29,30,21,12;,
  3;12,21,22;,
  3;14,24,23;,
  3;31,32,8;,
  3;8,32,33;,
  3;32,34,33;,
  3;34,25,33;,
  4;35,33,25,26;,
  3;14,23,16;,
  3;36,37,22;,
  3;22,37,14;,
  3;37,24,14;,
  4;5,27,15,6;,
  3;5,13,27;,
  3;4,13,5;,
  3;12,14,13;,
  3;12,22,14;,
  3;21,36,22;,
  4;13,14,15,27;,
  4;38,39,39,38;,
  4;39,39,31,8;,
  3;26,40,35;,
  3;35,40,41;,
  3;40,42,41;,
  3;43,44,45;,
  3;45,44,46;,
  3;44,47,48;,
  3;47,49,48;,
  3;48,46,44;;
 }
 MeshTextureCoords {
  28;
  0.668560;0.500000;,
  0.668560;0.577430;,
  0.704740;0.500000;,
  0.704740;0.625000;,
  0.668560;0.625000;,
  0.740910;0.625000;,
  0.740910;0.500000;,
  0.777080;0.625000;,
  0.777080;0.500000;,
  0.813260;0.625000;,
  0.813260;0.500000;,
  0.849430;0.589440;,
  0.849430;0.625000;,
  0.885600;0.625000;,
  0.885600;0.578320;,
  0.668560;0.750000;,
  0.704740;0.750000;,
  0.740910;0.750000;,
  0.777080;0.750000;,
  0.813260;0.750000;,
  0.849430;0.691060;,
  0.885600;0.722290;,
  0.885600;0.662120;,
  0.885600;0.750000;,
  0.849430;0.750000;,
  0.849430;0.500000;,
  0.871990;0.548840;,
  0.885600;0.500000;;
 }
}