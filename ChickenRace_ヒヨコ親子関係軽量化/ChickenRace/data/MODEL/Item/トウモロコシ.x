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
 193;
 -3.31247;8.50922;14.07801;,
 -4.90379;5.93848;12.51838;,
 -3.84104;5.93848;-15.34953;,
 -2.70650;8.67750;-15.34953;,
 0.03253;12.16633;-7.78510;,
 0.03253;9.81204;-15.34953;,
 4.03947;8.91255;15.62139;,
 2.77156;8.67750;-15.34953;,
 4.96885;5.93848;12.51838;,
 3.90610;5.93848;-15.34953;,
 4.07650;3.00142;15.64500;,
 2.77156;3.19945;-15.34953;,
 0.03253;-0.28939;-7.78510;,
 0.03253;2.06490;-15.34953;,
 -4.97280;3.96278;16.25811;,
 -2.70650;3.19945;-15.34953;,
 -4.90379;5.93848;12.51838;,
 -3.84104;5.93848;-15.34953;,
 -4.27144;5.93848;-15.34953;,
 -3.01084;8.98184;-15.34953;,
 0.03253;10.24244;-15.34953;,
 3.07589;8.98184;-15.34953;,
 4.33649;5.93848;-15.34953;,
 3.07589;2.89511;-15.34953;,
 0.03253;1.63451;-15.34953;,
 -3.01084;2.89511;-15.34953;,
 -4.27144;5.93848;-15.34953;,
 -4.37124;10.34224;0.06234;,
 -6.19533;5.93848;0.06234;,
 -5.45227;5.93848;12.51838;,
 -3.70031;8.89705;14.07801;,
 0.03253;12.16633;-7.78510;,
 4.43629;10.34224;0.06234;,
 4.42730;9.30039;15.62137;,
 6.26039;5.93848;0.06234;,
 5.51734;5.93848;12.51838;,
 4.43629;1.53471;0.06234;,
 4.46433;2.61358;15.64500;,
 0.03253;-0.28939;-7.78510;,
 -4.37124;1.53471;0.06234;,
 -5.36063;3.57495;16.25811;,
 -6.19533;5.93848;0.06234;,
 -5.45227;5.93848;12.51838;,
 -4.90379;5.93848;12.51838;,
 -3.31247;8.50922;14.07801;,
 4.03947;8.91255;15.62139;,
 4.96885;5.93848;12.51838;,
 4.07650;3.00142;15.64500;,
 -4.97280;3.96278;16.25811;,
 -4.90379;5.93848;12.51838;,
 -2.13755;3.81701;16.78607;,
 -2.99586;6.02431;16.86313;,
 -3.49209;2.51110;2.77709;,
 -4.02948;6.04211;9.77431;,
 -5.32151;6.04001;2.77709;,
 -2.81481;9.30307;11.99864;,
 -4.02948;6.04211;9.77431;,
 -2.99586;6.02431;16.86313;,
 -2.13755;8.27141;16.78607;,
 3.15912;9.30307;11.99864;,
 4.37378;6.04211;9.77433;,
 5.66225;6.04001;2.77709;,
 3.83282;9.56893;2.77709;,
 2.48540;8.27141;16.78607;,
 0.17393;8.84944;16.76722;,
 0.17531;8.11465;18.50292;,
 1.70152;7.49537;18.50293;,
 3.08531;6.04421;16.77155;,
 2.32304;6.04523;18.50293;,
 1.70152;4.59509;18.50293;,
 2.48540;3.81701;16.78607;,
 0.17531;3.97581;18.50292;,
 -1.30853;4.59509;18.50293;,
 -2.13755;3.81701;16.78607;,
 0.17393;3.23898;16.77155;,
 -1.45643;7.35287;18.50293;,
 -2.13755;8.27141;16.78607;,
 -2.99586;6.02431;16.86313;,
 -1.97242;6.04523;18.50293;,
 3.83282;2.51110;2.77709;,
 0.17037;0.59590;2.77709;,
 0.03253;2.06490;-15.34953;,
 3.11733;3.20051;-15.07797;,
 0.17393;3.23898;16.77155;,
 -2.13755;3.81701;16.78607;,
 -3.49209;2.51110;2.77709;,
 0.17037;0.59590;2.77709;,
 2.48540;3.81701;16.78607;,
 0.17393;3.23898;16.77155;,
 0.17037;0.59590;2.77709;,
 3.83282;2.51110;2.77709;,
 3.11733;3.20051;-15.07797;,
 0.03253;2.06490;-15.34953;,
 0.17037;6.04001;-15.41228;,
 3.90610;5.93848;-15.34953;,
 -2.77659;3.20051;-15.07797;,
 -3.84104;5.93848;-15.34953;,
 -2.77659;8.87952;-15.07797;,
 0.03253;9.81204;-15.34953;,
 3.11733;8.87952;-15.07797;,
 -2.77659;3.20051;-15.07797;,
 -3.49209;2.51110;2.77709;,
 -5.32151;6.04001;2.77709;,
 -3.50284;6.04001;-15.07797;,
 -3.49209;9.56893;2.77709;,
 -2.77659;8.87952;-15.07797;,
 3.83282;2.51110;2.77709;,
 3.11733;3.20051;-15.07797;,
 3.84356;6.04001;-15.07797;,
 5.66225;6.04001;2.77709;,
 3.11733;8.87952;-15.07797;,
 3.83282;9.56893;2.77709;,
 3.11733;8.87952;-15.07797;,
 0.03253;9.81204;-15.34953;,
 0.17037;11.48413;2.77709;,
 3.83282;9.56893;2.77709;,
 -2.77659;8.87952;-15.07797;,
 -3.49209;9.56893;2.77709;,
 -2.81481;9.30307;11.99864;,
 0.17215;10.54969;11.98923;,
 3.15912;9.30307;11.99864;,
 2.48540;3.81701;16.78607;,
 3.83282;2.51110;2.77709;,
 5.66225;6.04001;2.77709;,
 3.08531;6.04421;16.77155;,
 0.17531;8.11465;18.50292;,
 0.17553;6.04553;18.95894;,
 0.17553;6.04553;18.95894;,
 0.17553;6.04553;18.95894;,
 2.32304;6.04523;18.50293;,
 0.17553;6.04553;18.95894;,
 0.17531;3.97581;18.50292;,
 -1.97242;6.04523;18.50293;,
 -2.13755;8.27141;16.78607;,
 0.17393;8.84944;16.76722;,
 2.48540;8.27141;16.78607;,
 3.08531;6.04421;16.77155;,
 2.48540;8.27141;16.78607;,
 -5.32151;6.04001;2.77709;,
 -3.49209;9.56893;2.77709;,
 0.17037;0.59590;2.77709;,
 -3.49209;2.51110;2.77709;,
 -2.77659;3.20051;-15.07797;,
 0.17037;2.50075;-15.07797;,
 1.90356;3.64770;-18.62742;,
 0.10895;3.64770;-18.62743;,
 0.10895;5.75910;-18.62742;,
 1.90356;5.75910;-18.62742;,
 -1.68565;3.64770;-18.62742;,
 -1.68565;5.75910;-18.62742;,
 -1.68565;7.87049;-18.62743;,
 0.10895;7.87049;-18.62742;,
 1.90356;7.87049;-18.62742;,
 1.89910;3.65460;-15.30529;,
 1.90134;3.65115;-16.96635;,
 1.90134;5.75985;-16.96635;,
 1.89910;5.76060;-15.30529;,
 1.90356;3.64770;-18.62742;,
 1.90356;5.75910;-18.62742;,
 1.90356;7.87049;-18.62742;,
 1.90134;7.86855;-16.96635;,
 1.89910;7.86661;-15.30529;,
 -1.68565;3.64770;-18.62742;,
 -1.68329;3.65115;-16.96635;,
 -1.68329;5.75985;-16.96635;,
 -1.68565;5.75910;-18.62742;,
 -1.68094;3.65460;-15.30529;,
 -1.68094;5.76060;-15.30529;,
 -1.68094;7.86661;-15.30529;,
 -1.68329;7.86855;-16.96635;,
 -1.68565;7.87049;-18.62743;,
 1.90356;7.88658;-18.62742;,
 0.10895;7.88658;-18.62742;,
 0.10902;7.88464;-16.96634;,
 1.90134;7.88464;-16.96635;,
 -1.68565;7.88658;-18.62743;,
 -1.68329;7.88464;-16.96635;,
 -1.68094;7.88270;-15.30529;,
 0.10908;7.88270;-15.30529;,
 1.89910;7.88270;-15.30529;,
 1.89910;3.65460;-15.30529;,
 0.10908;3.65460;-15.30529;,
 0.10902;3.65115;-16.96635;,
 1.90134;3.65115;-16.96635;,
 -1.68094;3.65460;-15.30529;,
 -1.68329;3.65115;-16.96635;,
 -1.68565;3.64770;-18.62742;,
 0.10895;3.64770;-18.62743;,
 1.90356;3.64770;-18.62742;,
 0.17393;8.84944;16.76722;,
 -2.99586;6.02431;16.86313;,
 3.08531;6.04421;16.77155;,
 0.17393;3.23898;16.77155;;
 
 102;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;12,10,11,13;,
 4;14,12,13,15;,
 4;16,14,15,17;,
 4;3,2,18,19;,
 4;5,3,19,20;,
 4;7,5,20,21;,
 4;9,7,21,22;,
 4;11,9,22,23;,
 4;13,11,23,24;,
 4;15,13,24,25;,
 4;17,15,25,26;,
 4;27,28,29,30;,
 3;31,27,30;,
 3;32,31,33;,
 4;34,32,33,35;,
 4;36,34,35,37;,
 3;38,36,37;,
 3;39,38,40;,
 4;41,39,40,42;,
 4;30,29,43,44;,
 3;31,30,44;,
 3;33,31,45;,
 4;35,33,45,46;,
 4;37,35,46,47;,
 3;38,37,47;,
 3;40,38,48;,
 4;42,40,48,49;,
 4;26,25,39,41;,
 4;25,24,38,39;,
 4;24,23,36,38;,
 4;23,22,34,36;,
 4;22,21,32,34;,
 4;21,20,31,32;,
 4;20,19,27,31;,
 4;19,18,28,27;,
 3;50,51,52;,
 3;51,53,52;,
 3;53,54,52;,
 4;55,56,57,58;,
 4;59,60,61,62;,
 4;63,64,65,66;,
 4;67,68,69,70;,
 4;71,72,73,74;,
 4;75,76,77,78;,
 4;79,80,81,82;,
 4;83,84,85,86;,
 4;87,88,89,90;,
 4;91,92,93,94;,
 4;92,95,96,93;,
 4;93,96,97,98;,
 4;94,93,98,99;,
 4;100,101,102,103;,
 4;103,102,104,105;,
 4;106,107,108,109;,
 4;109,108,110,111;,
 4;112,113,114,115;,
 4;113,116,117,114;,
 4;114,117,118,119;,
 4;115,114,119,120;,
 4;121,122,123,124;,
 3;125,75,126;,
 3;127,72,71;,
 3;128,69,68;,
 3;129,66,130;,
 3;126,75,78;,
 3;131,69,128;,
 3;130,66,65;,
 3;127,132,72;,
 4;119,118,133,134;,
 4;120,119,134,135;,
 4;136,60,59,137;,
 4;138,56,55,139;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;145,148,149,146;,
 4;146,149,150,151;,
 4;147,146,151,152;,
 4;153,154,155,156;,
 4;154,157,158,155;,
 4;155,158,159,160;,
 4;156,155,160,161;,
 4;162,163,164,165;,
 4;163,166,167,164;,
 4;164,167,168,169;,
 4;165,164,169,170;,
 4;171,172,173,174;,
 4;172,175,176,173;,
 4;173,176,177,178;,
 4;174,173,178,179;,
 4;180,181,182,183;,
 4;181,184,185,182;,
 4;182,185,186,187;,
 4;183,182,187,188;,
 4;189,76,75,125;,
 4;73,72,132,190;,
 4;63,66,129,191;,
 4;70,69,131,192;;
 
 MeshMaterialList {
  1;
  102;
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
   TextureFilename {
    "トウモロコシ\\トウモロコシ.jpg";
   }
  }
 }
 MeshNormals {
  127;
  0.992363;-0.116189;0.041428;,
  0.761090;-0.642389;0.089882;,
  0.593158;-0.790982;0.150034;,
  -0.788033;-0.606763;0.104134;,
  -0.999155;-0.001559;0.041062;,
  -0.788943;0.605534;0.104393;,
  -0.575825;0.801529;0.161176;,
  0.822873;0.556589;0.114404;,
  0.000000;0.000000;-1.000000;,
  -0.994676;0.094506;0.041095;,
  -0.617270;0.780108;0.102026;,
  0.685513;0.725817;0.057113;,
  0.999223;0.001351;0.039402;,
  0.685571;-0.725730;0.057512;,
  0.269868;-0.939379;0.211516;,
  -0.998016;0.047844;-0.040920;,
  -0.581919;-0.810550;-0.066173;,
  0.715419;-0.696778;-0.051731;,
  0.999133;0.000684;-0.041633;,
  0.715360;0.696829;-0.051853;,
  -0.685488;0.727479;-0.029657;,
  -0.647225;-0.686379;0.331637;,
  -0.944794;-0.102821;0.311115;,
  -0.999025;-0.020510;0.039106;,
  -0.782182;-0.618523;0.074967;,
  -0.647011;0.658990;0.383548;,
  -0.722051;0.691658;0.015858;,
  0.948694;-0.029915;0.314779;,
  0.999454;0.005707;0.032528;,
  0.719730;0.694093;0.014968;,
  0.667289;0.647714;0.367685;,
  -0.001971;-0.001676;0.999997;,
  -0.005385;0.927848;0.372920;,
  0.666489;-0.675590;0.315231;,
  -0.001114;-0.953084;0.302704;,
  0.715259;-0.697742;0.039511;,
  0.015833;-0.999122;0.038784;,
  0.724778;-0.685083;-0.073203;,
  0.056592;0.058611;-0.996676;,
  -0.059357;0.055741;-0.996679;,
  -0.057132;-0.058051;-0.996678;,
  -0.997219;-0.000000;-0.074525;,
  0.997219;-0.000000;-0.074525;,
  -0.001390;-0.001161;-0.999998;,
  -0.003432;0.999988;0.003366;,
  -0.009483;0.604123;0.796835;,
  0.601134;0.000016;0.799148;,
  -0.001882;-0.608537;0.793523;,
  -0.561706;-0.009551;0.827282;,
  -0.400522;0.413680;0.817589;,
  0.411120;-0.420542;0.808780;,
  0.411224;0.420902;0.808539;,
  -0.387503;-0.427239;0.816890;,
  -0.705927;0.685860;0.176811;,
  0.000007;0.981777;0.190035;,
  0.716328;0.674006;0.180527;,
  -0.986234;-0.044773;0.159177;,
  0.937549;0.313083;0.151594;,
  -0.348833;-0.934491;-0.071012;,
  0.000003;-0.000003;-1.000000;,
  0.000003;0.000003;-1.000000;,
  -0.000003;0.000003;-1.000000;,
  -0.000000;0.000003;-1.000000;,
  0.000002;0.000001;-1.000000;,
  0.000000;-0.000000;-1.000000;,
  0.000002;-0.000001;-1.000000;,
  0.000001;0.000001;-1.000000;,
  0.999999;0.000000;0.001349;,
  0.999999;0.000000;0.001343;,
  0.999999;0.000000;0.001349;,
  -0.999999;0.000000;0.001415;,
  -0.999999;0.000000;0.001415;,
  -0.999999;0.000000;0.001418;,
  -0.999999;0.000000;0.001418;,
  -0.999999;0.000000;0.001418;,
  0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  -0.000000;0.999999;0.001168;,
  0.000000;-0.999998;0.002077;,
  0.000000;-0.999998;0.002077;,
  -0.575875;-0.801499;0.161143;,
  0.574589;0.802257;0.161959;,
  -0.029158;-0.000578;-0.999575;,
  -0.000692;0.028624;-0.999590;,
  0.027773;-0.000580;-0.999614;,
  -0.000695;-0.029781;-0.999556;,
  0.173187;0.984842;0.009632;,
  0.012762;-0.998128;0.059821;,
  -0.968272;-0.249863;0.004304;,
  -0.168262;-0.405249;0.898588;,
  0.689634;0.689652;0.220872;,
  -0.690692;0.690682;0.214252;,
  0.257991;-0.633179;0.729743;,
  0.261371;0.640496;0.722115;,
  -0.690406;-0.690383;0.216128;,
  0.682271;-0.682272;0.262699;,
  -0.341483;0.812255;0.472896;,
  -0.992298;0.000000;-0.123870;,
  -0.734633;-0.660699;-0.154242;,
  0.000001;-0.979889;-0.199543;,
  0.734633;-0.660699;-0.154242;,
  0.992298;0.000001;-0.123871;,
  0.734632;0.660700;-0.154242;,
  0.000001;0.979889;-0.199542;,
  -0.734632;0.660700;-0.154242;,
  0.407404;-0.910886;-0.065634;,
  0.054357;-0.060929;-0.996661;,
  -0.702578;-0.707411;-0.077155;,
  -0.719396;0.690520;-0.075182;,
  0.714644;0.695245;-0.076922;,
  -0.006785;0.997071;-0.076184;,
  0.999999;0.000000;0.001337;,
  0.999999;0.000000;0.001337;,
  -0.999999;0.000000;0.001421;,
  -0.999999;0.000000;0.001421;,
  -0.999999;0.000000;0.001421;,
  -0.000000;-0.999998;0.002077;,
  0.000000;-0.999998;0.002077;,
  0.000000;-0.999998;0.002077;,
  0.000000;-0.999998;0.002077;,
  -0.000000;-0.999998;0.002077;;
  102;
  4;1,0,0,1;,
  4;2,1,1,2;,
  4;3,86,86,3;,
  4;4,3,3,4;,
  4;5,4,4,5;,
  4;6,5,5,6;,
  4;7,87,87,7;,
  4;0,7,7,0;,
  4;8,88,8,8;,
  4;89,8,8,8;,
  4;8,89,8,8;,
  4;90,8,8,8;,
  4;8,90,8,8;,
  4;91,8,8,8;,
  4;8,91,8,8;,
  4;88,8,8,8;,
  4;20,15,9,10;,
  3;92,20,10;,
  3;19,92,11;,
  4;18,19,11,12;,
  4;17,18,12,13;,
  3;93,17,13;,
  3;16,93,14;,
  4;15,94,94,9;,
  4;95,95,95,95;,
  3;92,96,96;,
  3;97,97,97;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  3;93,100,100;,
  3;14,93,101;,
  4;102,102,102,102;,
  4;103,104,16,15;,
  4;104,105,93,16;,
  4;105,106,17,93;,
  4;106,107,18,17;,
  4;107,108,19,18;,
  4;108,109,92,19;,
  4;109,110,20,92;,
  4;110,103,15,20;,
  3;21,22,24;,
  3;22,56,24;,
  3;56,23,24;,
  4;53,56,22,25;,
  4;55,57,28,29;,
  4;30,32,45,51;,
  4;27,46,50,33;,
  4;47,52,21,34;,
  4;49,25,22,48;,
  4;35,36,111,37;,
  4;34,21,24,36;,
  4;33,34,36,35;,
  4;112,91,43,90;,
  4;91,40,88,43;,
  4;43,88,39,89;,
  4;90,43,89,38;,
  4;113,24,23,41;,
  4;41,23,26,114;,
  4;35,37,42,28;,
  4;28,42,115,29;,
  4;115,116,44,29;,
  4;116,114,26,44;,
  4;44,26,53,54;,
  4;29,44,54,55;,
  4;33,35,28,27;,
  3;45,49,31;,
  3;31,52,47;,
  3;31,50,46;,
  3;46,51,31;,
  3;31,49,48;,
  3;47,50,31;,
  3;31,51,45;,
  3;31,48,52;,
  4;54,53,25,32;,
  4;55,54,32,30;,
  4;27,57,55,30;,
  4;23,56,53,26;,
  4;36,24,113,58;,
  4;60,62,66,63;,
  4;62,61,64,66;,
  4;66,64,59,65;,
  4;63,66,65,8;,
  4;67,68,68,69;,
  4;68,117,117,68;,
  4;68,117,118,68;,
  4;69,68,68,67;,
  4;119,72,74,120;,
  4;72,70,71,74;,
  4;74,71,71,73;,
  4;120,74,73,121;,
  4;77,82,83,79;,
  4;82,78,80,83;,
  4;83,80,76,81;,
  4;79,83,81,75;,
  4;84,84,85,122;,
  4;84,84,123,85;,
  4;85,123,124,125;,
  4;122,85,125,126;,
  4;32,25,49,45;,
  4;21,52,48,22;,
  4;30,51,46,27;,
  4;33,50,47,34;;
 }
 MeshTextureCoords {
  193;
  0.472870;0.051430;,
  0.451430;0.051430;,
  0.451840;0.277380;,
  0.472870;0.275720;,
  0.494310;0.078000;,
  0.494390;0.324120;,
  0.515740;0.051430;,
  0.515740;0.275720;,
  0.537180;0.051430;,
  0.537170;0.364870;,
  0.558620;0.051430;,
  0.558620;0.275720;,
  0.580060;0.078000;,
  0.580250;0.338900;,
  0.601500;0.051430;,
  0.601500;0.275720;,
  0.622940;0.051430;,
  0.623700;0.325230;,
  0.451430;0.500000;,
  0.472870;0.500000;,
  0.494310;0.500000;,
  0.515740;0.500000;,
  0.537180;0.500000;,
  0.558620;0.500000;,
  0.580060;0.500000;,
  0.601500;0.500000;,
  0.622940;0.500000;,
  0.472870;0.612810;,
  0.451430;0.612810;,
  0.451430;0.724280;,
  0.472870;0.724280;,
  0.494310;0.659350;,
  0.515740;0.612810;,
  0.515740;0.724280;,
  0.537180;0.612810;,
  0.537180;0.724280;,
  0.558620;0.612810;,
  0.558620;0.724280;,
  0.580060;0.659350;,
  0.601500;0.612810;,
  0.601500;0.724280;,
  0.622940;0.612810;,
  0.622940;0.724280;,
  0.451430;0.948570;,
  0.472870;0.948570;,
  0.515740;0.948570;,
  0.537180;0.948570;,
  0.558620;0.948570;,
  0.601500;0.948570;,
  0.622940;0.948570;,
  0.181150;0.378800;,
  0.186780;0.007360;,
  0.005180;0.379350;,
  0.111810;0.007360;,
  0.036840;0.007360;,
  0.138750;-0.005030;,
  0.099150;0.335480;,
  0.174110;0.335480;,
  0.185370;0.106900;,
  0.120190;0.006750;,
  0.089730;0.338840;,
  0.177070;0.338840;,
  0.177070;0.113780;,
  0.017670;0.380050;,
  0.180210;0.376790;,
  0.181340;0.194650;,
  0.149470;0.103580;,
  0.013980;0.373380;,
  0.095070;0.373380;,
  0.138460;0.282020;,
  0.045670;0.025480;,
  0.030960;0.195270;,
  0.064610;0.285140;,
  0.181740;0.013610;,
  0.046370;0.015550;,
  0.072530;0.091930;,
  0.180120;0.379460;,
  0.172870;0.008340;,
  0.108000;0.008340;,
  0.011210;0.375900;,
  0.183990;0.375900;,
  0.190350;0.002900;,
  0.045180;0.008490;,
  0.015260;0.373400;,
  0.182910;0.374430;,
  0.183180;0.002750;,
  0.037980;0.004370;,
  0.005220;0.371190;,
  0.187510;0.379890;,
  0.187510;0.002750;,
  0.040140;0.002750;,
  0.396070;0.876470;,
  0.519520;0.910600;,
  0.519160;0.776260;,
  0.372360;0.790980;,
  0.661150;0.891880;,
  0.692710;0.790320;,
  0.654000;0.666460;,
  0.525130;0.639200;,
  0.384310;0.666460;,
  0.051560;0.975440;,
  0.201500;1.000000;,
  0.201500;0.875000;,
  0.051560;0.875000;,
  0.201500;0.750000;,
  0.051560;0.774550;,
  0.187670;0.961070;,
  0.346060;0.957060;,
  0.327230;0.488540;,
  0.187620;0.488540;,
  0.327220;0.109550;,
  0.187620;0.016010;,
  0.038110;0.997080;,
  0.193770;1.004620;,
  0.186840;0.500170;,
  0.001710;0.500170;,
  0.335400;0.997050;,
  0.371970;0.500170;,
  0.353920;0.247790;,
  0.186840;0.251710;,
  0.019770;0.247790;,
  0.024930;0.383300;,
  0.184080;0.387630;,
  0.184040;0.008280;,
  0.044430;0.008280;,
  0.029340;0.193750;,
  0.043130;0.008340;,
  0.015550;0.375000;,
  0.176160;0.373380;,
  0.117600;0.012500;,
  0.182470;0.012500;,
  0.181840;0.190660;,
  0.098260;0.375000;,
  0.333640;0.000920;,
  0.186840;0.003260;,
  0.040050;0.000920;,
  0.002380;0.338840;,
  0.040270;0.115060;,
  0.024170;0.335480;,
  0.049510;0.116770;,
  0.016890;0.380760;,
  0.189670;0.380760;,
  0.189670;0.010110;,
  0.016890;0.010110;,
  0.697080;0.456000;,
  0.845280;0.456000;,
  0.845280;0.331000;,
  0.697080;0.331000;,
  0.993480;0.456000;,
  0.993480;0.331000;,
  0.993480;0.206000;,
  0.845280;0.206000;,
  0.697080;0.206000;,
  0.699290;0.442230;,
  0.846010;0.442230;,
  0.846010;0.317230;,
  0.699290;0.317230;,
  0.992730;0.442230;,
  0.992730;0.317230;,
  0.992730;0.192230;,
  0.846010;0.192230;,
  0.699290;0.192230;,
  0.698400;0.462010;,
  0.848090;0.462010;,
  0.848090;0.337010;,
  0.698400;0.337010;,
  0.997770;0.462010;,
  0.997770;0.337010;,
  0.997770;0.212010;,
  0.848090;0.212010;,
  0.698400;0.212010;,
  0.697080;0.455710;,
  0.845280;0.455710;,
  0.845280;0.330710;,
  0.697080;0.330710;,
  0.993480;0.455710;,
  0.993480;0.330710;,
  0.993480;0.205710;,
  0.845280;0.205710;,
  0.697080;0.205710;,
  0.697080;0.457920;,
  0.845280;0.457920;,
  0.845280;0.332920;,
  0.697080;0.332920;,
  0.993480;0.457920;,
  0.993480;0.332920;,
  0.993480;0.207920;,
  0.845280;0.207920;,
  0.697080;0.207920;,
  0.015550;0.379150;,
  0.180980;0.375000;,
  0.052730;0.012500;,
  0.187520;0.007940;;
 }
}
