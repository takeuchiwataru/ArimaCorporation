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
 112;
 -3.18276;0.02244;1.25229;,
 -1.97481;-0.13662;2.89829;,
 -1.88563;0.91156;2.87476;,
 -3.04995;1.06857;1.28370;,
 0.00133;-0.19210;3.53558;,
 -0.00394;0.85873;3.48159;,
 1.97601;-0.11697;2.89582;,
 1.87637;0.93027;2.87240;,
 3.18932;0.06434;1.21802;,
 3.03708;1.09887;1.27989;,
 3.19581;0.27609;-0.84741;,
 3.03484;1.30009;-0.68767;,
 2.09035;0.52440;-2.87982;,
 1.87051;1.45711;-2.27872;,
 -1.86047;0.50475;-2.87735;,
 -3.20138;0.24380;-0.83898;,
 -3.05219;1.26981;-0.68385;,
 -1.89148;1.43838;-2.27636;,
 -3.18276;0.02244;1.25229;,
 -3.04995;1.06857;1.28370;,
 -1.60972;1.88543;2.58537;,
 -2.60016;2.01899;1.23194;,
 -0.00906;1.84050;3.10157;,
 1.59042;1.90136;2.58337;,
 2.57777;2.04476;1.22869;,
 2.57588;2.21594;-0.44500;,
 1.58544;2.34951;-1.79844;,
 -2.60205;2.19018;-0.44176;,
 -1.61470;2.33359;-1.79643;,
 -2.60016;2.01899;1.23194;,
 -2.60016;2.01899;1.23194;,
 -1.60972;1.88543;2.58537;,
 -1.17648;2.68942;2.06171;,
 -1.89608;2.78647;1.07840;,
 -0.00906;1.84050;3.10157;,
 -0.01354;2.65678;2.43676;,
 1.59042;1.90136;2.58337;,
 1.14855;2.70100;2.06026;,
 2.57777;2.04476;1.22869;,
 1.86591;2.80520;1.07604;,
 2.57588;2.21594;-0.44500;,
 1.86453;2.92955;-0.13998;,
 1.58544;2.34951;-1.79844;,
 1.14493;3.02659;-1.12330;,
 -0.01521;2.39445;-2.31464;,
 -0.01801;3.05925;-1.49834;,
 -1.61470;2.33359;-1.79643;,
 -1.18010;3.01503;-1.12184;,
 -2.60205;2.19018;-0.44176;,
 -1.89746;2.91084;-0.13762;,
 -2.60016;2.01899;1.23194;,
 -1.89608;2.78647;1.07840;,
 -0.62833;3.24484;1.35507;,
 -1.00664;3.29585;0.83811;,
 -0.01694;3.22768;1.55224;,
 0.59401;3.25092;1.35430;,
 0.97115;3.30570;0.83687;,
 0.97042;3.37108;0.19757;,
 0.59211;3.42209;-0.31940;,
 -0.01929;3.43927;-0.51657;,
 -0.63023;3.41602;-0.31863;,
 -1.00737;3.36125;0.19881;,
 -1.00664;3.29585;0.83811;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 -0.01892;3.49729;0.53459;,
 2.09035;0.52440;-2.87982;,
 0.11422;0.57987;-3.51710;,
 -0.01117;1.50993;-2.88555;,
 1.87051;1.45711;-2.27872;,
 -1.86047;0.50475;-2.87735;,
 -1.89148;1.43838;-2.27636;,
 -0.01521;2.39445;-2.31464;,
 1.58544;2.34951;-1.79844;,
 -1.61470;2.33359;-1.79643;,
 -3.18276;0.02244;1.25229;,
 -2.07312;-0.29152;3.01975;,
 -1.97481;-0.13662;2.89829;,
 0.00220;-0.34978;3.68902;,
 0.00133;-0.19210;3.53558;,
 1.97107;-0.26793;2.98331;,
 1.97601;-0.11697;2.89582;,
 3.18932;0.06434;1.21802;,
 3.19581;0.27609;-0.84741;,
 2.19034;0.33728;-3.03626;,
 2.09035;0.52440;-2.87982;,
 -1.95878;0.31662;-3.03366;,
 -3.20138;0.24380;-0.83898;,
 -1.86047;0.50475;-2.87735;,
 3.18932;0.06434;1.21802;,
 1.97107;-0.26793;2.98331;,
 3.19581;0.27609;-0.84741;,
 0.00220;-0.34978;3.68902;,
 2.19034;0.33728;-3.03626;,
 0.08797;0.51586;-3.81419;,
 -2.07312;-0.29152;3.01975;,
 -3.18276;0.02244;1.25229;,
 -1.95878;0.31662;-3.03366;,
 -3.20138;0.24380;-0.83898;,
 2.19034;0.33728;-3.03626;,
 0.08797;0.51586;-3.81419;,
 0.11422;0.57987;-3.51710;,
 2.09035;0.52440;-2.87982;,
 -1.95878;0.31662;-3.03366;,
 -1.86047;0.50475;-2.87735;;
 
 66;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;14,15,16,17;,
 4;15,18,19,16;,
 4;3,2,20,21;,
 4;2,5,22,20;,
 4;5,7,23,22;,
 4;7,9,24,23;,
 4;9,11,25,24;,
 4;11,13,26,25;,
 4;17,16,27,28;,
 4;16,19,29,27;,
 4;30,31,32,33;,
 4;31,34,35,32;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;33,32,52,53;,
 4;32,35,54,52;,
 4;35,37,55,54;,
 4;37,39,56,55;,
 4;39,41,57,56;,
 4;41,43,58,57;,
 4;43,45,59,58;,
 4;45,47,60,59;,
 4;47,49,61,60;,
 4;49,51,62,61;,
 3;53,52,63;,
 3;52,54,64;,
 3;54,55,65;,
 3;55,56,66;,
 3;56,57,67;,
 3;57,58,68;,
 3;58,59,69;,
 3;59,60,70;,
 3;60,61,71;,
 3;61,62,72;,
 4;73,74,75,76;,
 4;74,77,78,75;,
 4;76,75,79,80;,
 4;75,78,81,79;,
 3;82,83,84;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 3;87,89,88;,
 3;90,91,92;,
 3;93,94,95;,
 3;96,97,98;,
 3;97,99,98;,
 3;98,99,100;,
 3;100,99,101;,
 3;99,102,101;,
 3;102,103,101;,
 3;101,103,104;,
 3;103,105,104;,
 4;106,107,108,109;,
 4;107,110,111,108;;
 
 MeshMaterialList {
  1;
  66;
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  67;
  -0.854452;0.337699;0.394805;,
  -0.511933;0.419394;0.749689;,
  0.009174;0.374246;0.927284;,
  0.564355;0.276999;0.777673;,
  0.889693;0.190749;0.414803;,
  0.879893;0.390707;-0.270438;,
  0.536708;0.642567;-0.546857;,
  -0.005823;0.744532;-0.667561;,
  -0.534029;0.613609;-0.581634;,
  -0.877735;0.365853;-0.309406;,
  -0.906063;0.270489;0.325401;,
  -0.559677;0.225451;0.797454;,
  -0.000429;0.209467;0.977816;,
  0.559142;0.229949;0.796545;,
  0.903989;0.279729;0.323351;,
  0.898548;0.369544;-0.236748;,
  0.546826;0.501903;-0.670130;,
  -0.553269;0.463967;-0.691829;,
  -0.905842;0.341655;-0.250443;,
  -0.771362;0.556412;0.308878;,
  -0.477262;0.516753;0.710765;,
  -0.001964;0.503409;0.864046;,
  0.472987;0.521479;0.710171;,
  0.766174;0.564063;0.307912;,
  0.765609;0.614893;-0.189076;,
  0.471509;0.654556;-0.590962;,
  -0.478740;0.649826;-0.590368;,
  -0.771927;0.607242;-0.188111;,
  -0.561848;0.784273;0.263139;,
  -0.348344;0.755481;0.554892;,
  -0.003295;0.745796;0.666167;,
  0.341502;0.758914;0.554460;,
  0.554346;0.789828;0.262434;,
  0.553937;0.826727;-0.098366;,
  0.340433;0.855520;-0.390118;,
  -0.004616;0.865208;-0.501392;,
  -0.349417;0.852088;-0.389685;,
  -0.562259;0.821174;-0.097659;,
  -0.297745;0.935188;0.191758;,
  -0.185582;0.920061;0.345032;,
  -0.004300;0.914974;0.403490;,
  0.176845;0.921865;0.344806;,
  0.288660;0.938108;0.191386;,
  0.288447;0.957494;0.001825;,
  0.176288;0.972618;-0.151450;,
  -0.004988;0.977709;-0.209904;,
  -0.186141;0.970817;-0.151217;,
  -0.297960;0.954576;0.002206;,
  -0.004885;0.994798;0.101750;,
  -0.003790;0.667898;-0.744243;,
  -0.005462;0.551703;-0.834023;,
  -0.380626;0.707999;0.594862;,
  0.016818;0.650672;0.759172;,
  0.522165;0.470259;0.711477;,
  0.478225;0.779755;-0.404083;,
  -0.003881;-0.994661;-0.103121;,
  -0.458849;0.782725;-0.420475;,
  0.123168;-0.987251;-0.100828;,
  0.056507;-0.990840;-0.122650;,
  0.069027;-0.992029;-0.105418;,
  0.012709;-0.993712;-0.111240;,
  0.026141;-0.995306;-0.093175;,
  -0.026651;-0.991724;-0.125592;,
  -0.035980;-0.994771;-0.095579;,
  -0.022712;-0.996931;-0.074912;,
  -0.124699;-0.986798;-0.103342;,
  -0.003643;0.881905;-0.471414;;
  66;
  4;0,1,11,10;,
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;8,9,18,17;,
  4;9,0,10,18;,
  4;10,11,20,19;,
  4;11,12,21,20;,
  4;12,13,22,21;,
  4;13,14,23,22;,
  4;14,15,24,23;,
  4;15,16,25,24;,
  4;17,18,27,26;,
  4;18,10,19,27;,
  4;19,20,29,28;,
  4;20,21,30,29;,
  4;21,22,31,30;,
  4;22,23,32,31;,
  4;23,24,33,32;,
  4;24,25,34,33;,
  4;25,49,35,34;,
  4;49,26,36,35;,
  4;26,27,37,36;,
  4;27,19,28,37;,
  4;28,29,39,38;,
  4;29,30,40,39;,
  4;30,31,41,40;,
  4;31,32,42,41;,
  4;32,33,43,42;,
  4;33,34,44,43;,
  4;34,35,45,44;,
  4;35,36,46,45;,
  4;36,37,47,46;,
  4;37,28,38,47;,
  3;38,39,48;,
  3;39,40,48;,
  3;40,41,48;,
  3;41,42,48;,
  3;42,43,48;,
  3;43,44,48;,
  3;44,45,48;,
  3;45,46,48;,
  3;46,47,48;,
  3;47,38,48;,
  4;6,7,50,16;,
  4;7,8,17,50;,
  4;16,50,49,25;,
  4;50,17,26,49;,
  3;0,51,1;,
  4;51,52,2,1;,
  4;52,53,3,2;,
  3;53,4,3;,
  3;5,54,6;,
  3;56,9,8;,
  3;57,58,59;,
  3;58,60,59;,
  3;59,60,61;,
  3;61,60,55;,
  3;60,62,55;,
  3;62,63,55;,
  3;55,63,64;,
  3;63,65,64;,
  4;54,66,7,6;,
  4;66,56,8,7;;
 }
 MeshTextureCoords {
  112;
  0.074930;0.997040;,
  0.111020;0.997040;,
  0.111020;0.839420;,
  0.074930;0.839420;,
  0.147110;0.997040;,
  0.147110;0.839420;,
  0.183200;0.997040;,
  0.183200;0.839420;,
  0.219280;0.997040;,
  0.219280;0.839420;,
  0.255370;0.997040;,
  0.255370;0.839420;,
  0.291460;0.997040;,
  0.291460;0.839420;,
  0.003920;0.999330;,
  0.040010;1.002770;,
  0.040010;0.837480;,
  0.003920;0.840930;,
  0.076100;1.002770;,
  0.076100;0.837480;,
  0.111020;0.681800;,
  0.074930;0.681800;,
  0.147110;0.681800;,
  0.183200;0.681800;,
  0.219280;0.681800;,
  0.255370;0.681800;,
  0.291460;0.681800;,
  0.040010;0.682530;,
  0.003920;0.682530;,
  0.076100;0.682530;,
  0.025630;0.684710;,
  0.120120;0.684710;,
  0.120120;0.650830;,
  0.025630;0.650830;,
  0.214610;0.684710;,
  0.214610;0.650830;,
  0.309100;0.684710;,
  0.309100;0.650830;,
  0.403590;0.684710;,
  0.403590;0.650830;,
  0.498080;0.684710;,
  0.498080;0.650830;,
  0.592570;0.685760;,
  0.592570;0.650830;,
  0.687060;0.685760;,
  0.687060;0.650830;,
  0.781550;0.685760;,
  0.781550;0.650830;,
  0.876040;0.684710;,
  0.876040;0.650830;,
  0.970530;0.684710;,
  0.970530;0.650830;,
  0.120120;0.616940;,
  0.025630;0.616940;,
  0.214610;0.616940;,
  0.309100;0.616940;,
  0.403590;0.616940;,
  0.498080;0.616940;,
  0.592570;0.616940;,
  0.687060;0.616940;,
  0.781550;0.616940;,
  0.876040;0.616940;,
  0.970530;0.616940;,
  0.072880;0.583060;,
  0.167370;0.583060;,
  0.261860;0.583060;,
  0.356350;0.583060;,
  0.450840;0.583060;,
  0.545330;0.583060;,
  0.639820;0.583060;,
  0.734310;0.583060;,
  0.828800;0.583060;,
  0.923290;0.583060;,
  0.334570;0.995420;,
  0.500120;0.995420;,
  0.500120;0.841180;,
  0.334570;0.841180;,
  0.665660;0.995420;,
  0.665660;0.841180;,
  0.500120;0.680870;,
  0.334570;0.680870;,
  0.665660;0.680870;,
  0.074930;0.597150;,
  0.111020;0.611690;,
  0.111020;0.582600;,
  0.147110;0.611690;,
  0.147110;0.582600;,
  0.183200;0.611690;,
  0.183200;0.582600;,
  0.219280;0.597150;,
  0.255370;0.597150;,
  0.291460;0.611690;,
  0.291460;0.582600;,
  0.003920;0.612120;,
  0.040010;0.597500;,
  0.003920;0.582880;,
  0.831890;0.462960;,
  0.768040;0.462960;,
  0.883540;0.469890;,
  0.716390;0.469890;,
  0.903270;0.481100;,
  0.883540;0.492300;,
  0.696660;0.481100;,
  0.716390;0.492300;,
  0.831890;0.499230;,
  0.768040;0.499230;,
  0.334570;0.611400;,
  0.500120;0.611400;,
  0.500120;0.582930;,
  0.334570;0.582930;,
  0.665660;0.611400;,
  0.665660;0.582930;;
 }
}