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
 165;
 197.63718;22.93980;-50.89994;,
 -155.42401;22.93980;-50.89996;,
 -153.77915;-1.05038;52.59493;,
 197.22162;-1.11355;52.24837;,
 -153.77915;-1.05038;52.59493;,
 -153.23076;-1.02741;-65.80637;,
 -153.70834;149.74111;-66.43816;,
 -153.23076;166.53425;50.93044;,
 -153.23076;166.53425;50.93044;,
 -171.47997;165.85485;51.06332;,
 -171.36441;-1.06910;52.84650;,
 -153.77915;-1.05038;52.59493;,
 -153.70834;149.74111;-66.43816;,
 -171.50664;148.99656;-66.45116;,
 -171.47997;165.85485;51.06332;,
 -153.23076;166.53425;50.93044;,
 -171.55408;-1.53186;-66.19459;,
 -171.36441;-1.06910;52.84650;,
 -171.47997;165.85485;51.06332;,
 196.75342;149.31182;-66.40632;,
 -171.50664;148.99656;-66.45116;,
 -171.56467;163.92134;-68.68956;,
 196.43170;163.90098;-68.74020;,
 -171.56636;182.53032;63.15908;,
 196.42834;182.35221;63.39936;,
 -171.46452;168.05452;65.05083;,
 196.42834;166.85975;65.81205;,
 -171.50664;148.99656;-66.45116;,
 196.75342;149.31182;-66.40632;,
 196.42834;166.85975;65.81205;,
 196.42834;182.35221;63.39936;,
 196.81187;-2.03631;-52.10893;,
 196.77621;150.03358;-52.12564;,
 -171.66196;-2.03630;-51.64206;,
 -155.40401;150.07720;-51.73358;,
 -171.67817;150.08492;-51.69276;,
 196.77621;150.03358;-52.12564;,
 196.75342;149.31182;-66.40632;,
 -153.70834;149.74111;-66.43816;,
 -155.40401;150.07720;-51.73358;,
 196.87946;-1.53186;-66.19496;,
 -171.55408;-1.53186;-66.19459;,
 196.75342;149.31182;-66.40632;,
 -153.70834;149.74111;-66.43816;,
 -171.50664;148.99656;-66.45116;,
 -171.50664;148.99656;-66.45116;,
 -171.67817;150.08492;-51.69276;,
 178.60780;-1.02741;-65.80637;,
 178.60780;-1.01963;52.86487;,
 178.50830;165.98941;51.08581;,
 178.45856;149.21999;-66.38936;,
 197.22162;-1.11355;52.24837;,
 196.87946;-1.53186;-66.19496;,
 196.75342;149.31182;-66.40632;,
 197.09328;150.55024;51.08192;,
 197.22162;-1.11355;52.24837;,
 197.09328;150.55024;51.08192;,
 178.60780;-1.01963;52.86487;,
 197.08022;165.99275;50.96314;,
 178.50830;165.98941;51.08581;,
 196.75342;149.31182;-66.40632;,
 178.45856;149.21999;-66.38936;,
 178.50830;165.98941;51.08581;,
 197.08022;165.99275;50.96314;,
 197.33723;152.98425;36.36112;,
 -171.28262;152.98425;36.36112;,
 -171.28262;165.45118;36.36112;,
 197.33723;165.45118;36.36112;,
 -171.47997;165.85485;51.06332;,
 197.08022;165.99275;50.96314;,
 -171.28262;152.98425;50.99585;,
 197.33723;152.98425;50.99585;,
 -171.28262;152.98425;50.99585;,
 -171.47997;165.85485;51.06332;,
 197.33723;-1.11857;38.19341;,
 -171.28262;-1.11857;38.19341;,
 -171.28262;11.34837;38.19341;,
 197.33723;11.34837;38.19341;,
 -171.28262;11.34837;52.82814;,
 197.33723;11.34837;52.82814;,
 -171.36441;-1.06910;52.84650;,
 197.22162;-1.11355;52.24837;,
 -171.36441;-1.06910;52.84650;,
 -171.28262;11.34837;52.82814;,
 197.08022;165.99275;50.96314;,
 -171.50664;148.99656;-66.45116;,
 -171.46452;168.05452;65.05083;,
 -171.56636;182.53032;63.15908;,
 -171.56467;163.92134;-68.68956;,
 120.82701;153.99477;65.12763;,
 -74.84689;154.08825;64.86520;,
 -74.85444;224.28683;64.75470;,
 120.91371;224.26238;64.83577;,
 120.90409;224.25195;67.85358;,
 -74.84751;224.25195;67.85358;,
 -74.84751;154.09513;67.85396;,
 120.90409;154.09513;67.85396;,
 120.91371;224.26238;64.83577;,
 -74.85444;224.28683;64.75470;,
 -74.84751;224.25195;67.85358;,
 120.90409;224.25195;67.85358;,
 120.90409;154.09513;67.85396;,
 -74.84751;154.09513;67.85396;,
 -74.84689;154.08825;64.86520;,
 120.82701;153.99477;65.12763;,
 -74.84689;154.08825;64.86520;,
 -74.84751;154.09513;67.85396;,
 -74.84751;224.25195;67.85358;,
 -74.85444;224.28683;64.75470;,
 120.90409;154.09513;67.85396;,
 120.82701;153.99477;65.12763;,
 120.91371;224.26238;64.83577;,
 120.90409;224.25195;67.85358;,
 24.00968;-0.45727;38.19341;,
 9.38275;-0.45727;38.19341;,
 9.38275;165.07481;38.19341;,
 24.00968;165.07481;38.19341;,
 9.38275;165.07481;52.82814;,
 24.00968;165.07481;52.82814;,
 9.38275;-0.45727;52.82814;,
 24.00968;-0.45727;52.82814;,
 9.38275;-0.45727;52.82814;,
 9.38275;165.07481;52.82814;,
 24.00968;-0.45727;52.82814;,
 24.00968;165.07481;52.82814;,
 -68.83241;-0.45727;38.19341;,
 -83.45934;-0.45727;38.19341;,
 -83.45934;165.07481;38.19341;,
 -68.83241;165.07481;38.19341;,
 -83.45934;165.07481;52.82814;,
 -68.83241;165.07481;52.82814;,
 -83.45934;-0.45727;52.82814;,
 -68.83241;-0.45727;52.82814;,
 -83.45934;-0.45727;52.82814;,
 -83.45934;165.07481;52.82814;,
 -68.83241;-0.45727;52.82814;,
 -68.83241;165.07481;52.82814;,
 110.97131;-0.45727;38.19341;,
 96.34439;-0.45727;38.19341;,
 96.34439;165.07481;38.19341;,
 110.97131;165.07481;38.19341;,
 96.34439;165.07481;52.82814;,
 110.97131;165.07481;52.82814;,
 96.34439;-0.45727;52.82814;,
 110.97131;-0.45727;52.82814;,
 96.34439;-0.45727;52.82814;,
 96.34439;165.07481;52.82814;,
 110.97131;-0.45727;52.82814;,
 110.97131;165.07481;52.82814;,
 -82.77499;153.72156;52.30925;,
 -153.23987;153.71776;52.30925;,
 -153.55994;11.48533;52.30925;,
 -83.09506;11.48918;52.30925;,
 9.58231;152.87842;52.30925;,
 -69.33884;152.89374;52.30925;,
 -69.65895;11.48533;52.30925;,
 9.26220;11.46998;52.30925;,
 96.50416;152.87842;52.30925;,
 23.78440;152.87970;52.30925;,
 23.46428;11.47126;52.30925;,
 96.18404;11.46998;52.30925;,
 179.06845;152.87842;52.30925;,
 110.22908;152.87092;52.30925;,
 109.90897;11.46251;52.30925;,
 178.74835;11.46998;52.30925;;
 
 59;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,13;,
 4;19,20,21,22;,
 4;22,21,23,24;,
 4;24,23,25,26;,
 4;26,25,27,28;,
 4;29,19,22,30;,
 3;31,32,33;,
 3;32,34,33;,
 3;34,35,33;,
 4;36,37,38,39;,
 3;40,41,42;,
 3;42,41,43;,
 3;41,44,43;,
 4;39,38,45,46;,
 4;47,48,49,50;,
 4;51,52,53,54;,
 3;55,56,57;,
 3;56,58,57;,
 3;58,59,57;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;67,66,68,69;,
 4;69,68,70,71;,
 4;65,72,73,66;,
 4;74,75,76,77;,
 4;77,76,78,79;,
 4;79,78,80,81;,
 4;75,82,83,76;,
 3;54,53,84;,
 4;85,86,87,88;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;97,98,99,100;,
 4;101,102,103,104;,
 4;105,106,107,108;,
 4;109,110,111,112;,
 4;113,114,115,116;,
 4;116,115,117,118;,
 4;118,117,119,120;,
 4;114,121,122,115;,
 4;123,113,116,124;,
 4;125,126,127,128;,
 4;128,127,129,130;,
 4;130,129,131,132;,
 4;126,133,134,127;,
 4;135,125,128,136;,
 4;137,138,139,140;,
 4;140,139,141,142;,
 4;142,141,143,144;,
 4;138,145,146,139;,
 4;147,137,140,148;,
 4;149,150,151,152;,
 4;153,154,155,156;,
 4;157,158,159,160;,
 4;161,162,163,164;;
 
 MeshMaterialList {
  1;
  59;
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
  }
 }
 MeshNormals {
  61;
  0.000200;0.974021;0.226457;,
  1.000000;-0.000060;0.000518;,
  -0.001065;0.141738;0.989904;,
  0.005708;-0.006106;0.999965;,
  -1.000000;-0.000185;0.000959;,
  -0.030189;0.994902;-0.096225;,
  0.000048;-0.052189;-0.998637;,
  0.000056;-0.153142;-0.988204;,
  0.000303;0.990289;-0.139023;,
  0.005369;0.004469;0.999976;,
  0.001190;0.000296;0.999999;,
  0.001267;0.000110;0.999999;,
  0.001811;0.000408;0.999998;,
  0.002508;0.000334;0.999997;,
  0.001629;0.000309;0.999999;,
  -0.000001;-0.001401;-0.999999;,
  -1.000000;-0.000793;0.000282;,
  0.013294;0.005670;0.999896;,
  0.000000;0.000000;-1.000000;,
  0.000135;-0.001362;0.999999;,
  0.000000;1.000000;0.000000;,
  0.999995;0.000826;-0.002899;,
  0.000877;-0.002865;-0.999996;,
  0.000000;0.000005;1.000000;,
  0.000000;0.000000;1.000000;,
  0.010602;0.010300;0.999891;,
  -0.019907;0.996879;-0.076395;,
  -0.019912;0.996002;-0.087089;,
  -0.039566;0.989130;-0.141623;,
  -0.999998;0.001554;-0.000916;,
  -0.999989;-0.003908;-0.002771;,
  -0.999996;-0.002760;0.000936;,
  0.000430;-0.077647;-0.996981;,
  -0.001339;-0.990501;0.137500;,
  0.999940;0.010973;-0.000289;,
  0.999989;0.004208;-0.002029;,
  0.000663;0.999327;-0.036662;,
  -0.001223;0.996035;-0.088956;,
  -0.010042;0.998997;-0.043643;,
  0.000297;-0.001576;-0.999999;,
  0.000445;-0.001663;-0.999999;,
  -0.020746;0.998503;-0.050617;,
  0.999995;0.000828;-0.002897;,
  0.016980;-0.007399;0.999828;,
  0.033139;0.007715;0.999421;,
  0.024296;0.008696;0.999667;,
  0.006602;0.010656;0.999921;,
  -0.003106;0.990004;-0.141005;,
  -0.001651;0.996228;-0.086761;,
  -0.000191;0.999480;-0.032257;,
  -0.999950;-0.007631;-0.006501;,
  0.000812;-0.022509;0.999746;,
  -0.999991;0.003293;-0.002791;,
  0.999995;0.000824;-0.002901;,
  -0.999985;-0.005334;0.000914;,
  0.000060;0.999973;0.007357;,
  -0.000240;-0.999809;0.019548;,
  -1.000000;-0.000054;0.001016;,
  0.999921;-0.000675;-0.012516;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  59;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;25,9,3,25;,
  4;26,5,27,28;,
  4;4,29,30,31;,
  4;6,32,7,7;,
  4;8,8,8,8;,
  4;2,2,2,2;,
  4;33,33,33,33;,
  4;34,35,34,34;,
  3;11,10,14;,
  3;10,12,14;,
  3;12,13,14;,
  4;36,37,26,38;,
  3;15,39,6;,
  3;6,39,40;,
  3;39,32,40;,
  4;38,26,5,41;,
  4;16,16,16,16;,
  4;42,42,35,21;,
  3;43,44,45;,
  3;44,17,45;,
  3;17,46,45;,
  4;37,47,47,48;,
  4;18,18,18,18;,
  4;49,49,27,48;,
  4;17,9,19,19;,
  4;50,50,30,50;,
  4;18,18,18,18;,
  4;20,20,20,20;,
  4;51,51,3,43;,
  4;52,29,52,52;,
  3;21,35,53;,
  4;31,54,54,54;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;18,18,18,18;,
  4;20,20,20,20;,
  4;24,24,24,24;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;18,18,18,18;,
  4;20,20,20,20;,
  4;24,24,24,24;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;18,18,18,18;,
  4;20,20,20,20;,
  4;24,24,24,24;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;24,24,24,24;,
  4;24,24,24,24;,
  4;24,24,24,24;,
  4;24,24,24,24;;
 }
 MeshTextureCoords {
  165;
  0.504848;0.993561;,
  1.000548;0.993561;,
  1.000548;0.499009;,
  0.513227;0.494819;,
  0.500134;0.499866;,
  0.995227;0.499866;,
  0.995227;0.002226;,
  0.500134;0.002226;,
  0.007098;0.495654;,
  0.495813;0.495654;,
  0.495813;0.007216;,
  0.007098;0.007216;,
  0.014474;0.254721;,
  0.226927;0.254721;,
  0.226927;0.095382;,
  0.014474;0.095382;,
  0.226927;0.414061;,
  0.441504;0.414061;,
  0.441504;0.254721;,
  0.183509;0.328563;,
  0.283633;0.328563;,
  0.283633;0.275515;,
  0.183509;0.275515;,
  0.283633;0.222467;,
  0.183509;0.222467;,
  0.283633;0.169419;,
  0.183509;0.169419;,
  0.283633;0.116370;,
  0.183509;0.116370;,
  0.084386;0.328563;,
  0.084386;0.275515;,
  0.997439;0.498311;,
  0.997439;0.003535;,
  0.501056;0.498311;,
  0.523000;0.003535;,
  0.501056;0.003535;,
  0.005075;0.492774;,
  0.496548;0.492774;,
  0.496548;0.029020;,
  0.005075;0.028723;,
  0.002155;0.493607;,
  0.493338;0.486625;,
  0.002155;0.007469;,
  0.471327;0.007469;,
  0.493338;0.007469;,
  0.496548;0.006946;,
  0.005075;0.006946;,
  0.501310;0.499385;,
  0.998910;0.499385;,
  0.998910;0.003411;,
  0.501310;0.003411;,
  0.338206;0.038504;,
  0.328914;0.158007;,
  0.488798;0.152372;,
  0.475710;0.043529;,
  0.195629;0.003027;,
  0.188024;0.450130;,
  0.318454;0.007216;,
  0.187249;0.495654;,
  0.318454;0.495654;,
  0.014474;0.288238;,
  0.226927;0.254721;,
  0.226927;0.095382;,
  0.014474;0.128898;,
  0.200115;0.251883;,
  0.262509;0.251883;,
  0.262509;0.205087;,
  0.200115;0.205087;,
  0.262509;0.158292;,
  0.200115;0.158292;,
  0.262509;0.111496;,
  0.200115;0.111496;,
  0.325527;0.251883;,
  0.325527;0.205087;,
  0.200115;0.251883;,
  0.262509;0.251883;,
  0.262509;0.205087;,
  0.200115;0.205087;,
  0.262509;0.158292;,
  0.200115;0.158292;,
  0.262509;0.111496;,
  0.208494;0.107306;,
  0.325527;0.251883;,
  0.325527;0.205087;,
  0.489711;0.044041;,
  0.048861;0.442328;,
  0.385528;0.442328;,
  0.385528;0.192328;,
  0.048861;0.192328;,
  0.021169;0.475091;,
  0.354502;0.475091;,
  0.354502;0.225091;,
  0.021169;0.225091;,
  0.000685;0.762720;,
  0.500499;0.764815;,
  0.499503;0.998653;,
  -0.000310;0.996559;,
  0.152961;0.248209;,
  0.486295;0.248209;,
  0.486295;-0.001791;,
  0.152961;-0.001791;,
  -0.000779;0.252191;,
  0.332554;0.252191;,
  0.332554;0.002192;,
  -0.000779;0.002192;,
  0.149668;0.250092;,
  0.486335;0.250092;,
  0.486335;0.000092;,
  0.149668;0.000092;,
  -0.000290;0.250342;,
  0.329710;0.250342;,
  0.329710;0.000342;,
  -0.000290;0.000342;,
  0.200115;0.251883;,
  0.262509;0.251883;,
  0.262509;0.205087;,
  0.200115;0.205087;,
  0.262509;0.158292;,
  0.200115;0.158292;,
  0.262509;0.111496;,
  0.200115;0.111496;,
  0.325527;0.251883;,
  0.325527;0.205087;,
  0.138344;0.251883;,
  0.138344;0.205087;,
  0.200115;0.251883;,
  0.262509;0.251883;,
  0.262509;0.205087;,
  0.200115;0.205087;,
  0.262509;0.158292;,
  0.200115;0.158292;,
  0.262509;0.111496;,
  0.200115;0.111496;,
  0.325527;0.251883;,
  0.325527;0.205087;,
  0.138344;0.251883;,
  0.138344;0.205087;,
  0.200115;0.251883;,
  0.262509;0.251883;,
  0.262509;0.205087;,
  0.200115;0.205087;,
  0.262509;0.158292;,
  0.200115;0.158292;,
  0.262509;0.111496;,
  0.200115;0.111496;,
  0.325527;0.251883;,
  0.325527;0.205087;,
  0.138344;0.251883;,
  0.138344;0.205087;,
  0.064621;0.752385;,
  0.397954;0.752385;,
  0.397954;0.502385;,
  0.064621;0.502385;,
  0.064621;0.754950;,
  0.397954;0.754950;,
  0.397954;0.504951;,
  0.064621;0.504951;,
  0.064621;0.749885;,
  0.397954;0.749885;,
  0.397954;0.499885;,
  0.064621;0.499885;,
  0.064621;0.752418;,
  0.397954;0.752418;,
  0.397954;0.502418;,
  0.064621;0.502418;;
 }
}
