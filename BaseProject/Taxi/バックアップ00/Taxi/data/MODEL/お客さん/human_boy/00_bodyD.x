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
 117;
 4.98657;4.04154;-2.74365;,
 2.55747;3.99038;-4.85337;,
 2.65683;5.44493;-4.91433;,
 5.06766;5.35917;-2.96139;,
 -0.40713;3.97909;-6.03045;,
 -0.63435;5.60859;-5.86821;,
 -0.08012;6.03613;3.56105;,
 -0.22890;4.38264;3.78606;,
 1.01472;4.37384;3.72009;,
 1.13976;6.09349;3.48463;,
 4.97708;6.12093;1.33030;,
 4.62447;4.30101;1.91070;,
 6.15960;4.21059;0.50154;,
 5.86465;5.89364;0.16467;,
 4.87839;2.75170;-2.54985;,
 2.60595;2.73213;-4.79223;,
 -0.26373;2.72617;-6.18393;,
 -0.32733;2.72815;3.96225;,
 0.97905;2.74967;4.08429;,
 2.77245;4.35594;2.95680;,
 2.79912;2.83848;3.20577;,
 4.54986;2.83573;2.07678;,
 5.75652;2.81031;0.40863;,
 4.91799;1.49823;-2.47809;,
 2.80215;1.49751;-4.83885;,
 -0.18708;1.53560;-6.40323;,
 -0.36621;1.24018;3.94320;,
 1.01163;1.39106;3.93573;,
 2.81388;1.48814;3.28719;,
 4.56504;1.50817;2.04942;,
 5.68899;1.51056;0.46245;,
 5.01456;0.08343;-2.86884;,
 2.86041;0.09479;-4.90953;,
 -0.13440;0.19101;-6.73065;,
 -0.36747;-0.34211;3.70275;,
 1.12608;-0.12321;4.04853;,
 3.04122;-0.00798;3.39660;,
 4.78920;0.02395;2.20278;,
 5.69823;0.06400;0.48072;,
 2.96640;6.13611;2.59895;,
 -3.77547;5.48715;-4.50168;,
 -3.38526;3.99819;-4.69677;,
 -5.71800;4.06371;-2.40288;,
 -5.90577;5.42572;-2.29665;,
 -1.47255;4.37366;3.76095;,
 -1.29495;6.05729;3.57983;,
 -6.50901;4.22793;0.72120;,
 -5.10084;4.31006;2.14167;,
 -4.23946;6.19230;2.74843;,
 -6.34776;5.71912;0.58818;,
 -6.50901;4.22793;0.72120;,
 -6.34776;5.71912;0.58818;,
 -3.18867;2.71935;-4.79226;,
 -5.47818;2.73558;-2.50785;,
 -1.64169;2.57933;4.00086;,
 -5.15082;2.67830;2.40894;,
 -3.41841;2.82171;3.17247;,
 -3.22560;4.35867;3.07911;,
 -6.33513;2.78870;0.43026;,
 -3.22281;1.47125;-4.93794;,
 -5.41611;1.46044;-2.63184;,
 -1.73814;1.50923;3.80868;,
 -5.20893;1.47060;1.88031;,
 -3.51021;1.47457;3.16170;,
 -6.27615;1.46570;0.26670;,
 -3.20373;0.06029;-5.04699;,
 -5.45163;0.23427;-2.57007;,
 -1.87023;-0.49992;3.73281;,
 -5.44752;-0.32062;2.16240;,
 -3.75453;-0.29052;3.22956;,
 -6.26985;0.01431;0.22146;,
 -6.33513;2.78870;0.43026;,
 -6.27615;1.46570;0.26670;,
 -6.26985;0.01431;0.22146;,
 -2.99476;6.08965;3.06527;,
 5.37930;5.60814;-1.54035;,
 -0.15510;5.56556;-1.28796;,
 -0.55713;6.06240;0.32096;,
 5.82246;5.89038;0.09519;,
 -0.64377;5.64049;-5.91576;,
 -3.67743;5.47866;-4.56099;,
 -5.83086;5.42762;-2.39619;,
 -6.08040;5.64194;-0.72363;,
 2.46966;5.45236;-4.98357;,
 1.79994;5.49993;-0.66885;,
 5.11362;5.31939;-2.88357;,
 -3.22766;5.88223;-0.43991;,
 -6.30469;5.72522;0.56738;,
 -4.22250;6.17043;2.80565;,
 -1.46827;6.05227;3.55157;,
 4.97617;6.12040;1.38914;,
 1.67917;6.00847;0.23378;,
 0.87471;5.95391;2.45009;,
 1.17392;6.07492;3.48794;,
 2.94427;6.17147;2.62778;,
 -1.02084;6.06676;1.95866;,
 5.41218;0.10768;-1.69014;,
 5.77830;0.09434;0.26508;,
 -0.53466;-0.11725;0.22452;,
 -0.10644;-0.02491;-1.43055;,
 -5.37981;0.29881;-2.73324;,
 -3.41325;0.10714;-4.97817;,
 0.07632;0.14872;-6.61950;,
 -6.24519;0.03602;0.20442;,
 2.94468;0.11436;-4.84902;,
 1.84857;-0.09058;-0.81144;,
 5.12310;0.06289;-2.64006;,
 -3.21798;-0.12804;-0.30744;,
 -3.63405;-0.30014;3.28623;,
 -5.45760;-0.31712;2.10561;,
 -1.87119;-0.38331;3.40047;,
 4.63683;0.06971;2.06469;,
 0.93726;-0.12217;2.38113;,
 1.69677;0.05399;0.12078;,
 2.87697;-0.02723;3.43458;,
 0.99087;-0.13770;3.93522;,
 -1.18521;-0.26943;1.85679;;
 
 78;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;10,11,12,13;,
 4;12,0,3,13;,
 4;14,15,1,0;,
 4;15,16,4,1;,
 4;7,17,18,8;,
 4;19,20,21,11;,
 4;22,14,0,12;,
 4;23,24,15,14;,
 4;24,25,16,15;,
 4;17,26,27,18;,
 4;20,28,29,21;,
 4;30,23,14,22;,
 4;31,32,24,23;,
 4;32,33,25,24;,
 4;34,35,27,26;,
 4;28,36,37,29;,
 4;38,31,23,30;,
 4;11,21,22,12;,
 4;21,29,30,22;,
 4;29,37,38,30;,
 4;11,10,39,19;,
 4;8,19,39,9;,
 4;8,18,20,19;,
 4;18,27,28,20;,
 4;27,35,36,28;,
 4;40,41,42,43;,
 4;5,4,41,40;,
 4;44,7,6,45;,
 4;46,47,48,49;,
 4;43,42,50,51;,
 4;41,52,53,42;,
 4;4,16,52,41;,
 4;54,17,7,44;,
 4;55,56,57,47;,
 4;42,53,58,50;,
 4;52,59,60,53;,
 4;16,25,59,52;,
 4;61,26,17,54;,
 4;62,63,56,55;,
 4;53,60,64,58;,
 4;59,65,66,60;,
 4;25,33,65,59;,
 4;61,67,34,26;,
 4;68,69,63,62;,
 4;60,66,70,64;,
 4;71,55,47,46;,
 4;72,62,55,71;,
 4;73,68,62,72;,
 4;74,48,47,57;,
 4;74,57,44,45;,
 4;56,54,44,57;,
 4;63,61,54,56;,
 4;69,67,61,63;,
 4;75,76,77,78;,
 4;76,79,80,81;,
 4;76,81,82,77;,
 4;83,79,76,84;,
 4;83,84,75,85;,
 4;86,82,87,88;,
 4;86,88,89,77;,
 4;90,78,91,92;,
 4;90,92,93,94;,
 4;92,91,77,95;,
 4;92,95,89,93;,
 4;96,97,98,99;,
 4;99,100,101,102;,
 4;99,98,103,100;,
 4;104,105,99,102;,
 4;104,106,96,105;,
 4;107,108,109,103;,
 4;107,98,110,108;,
 4;111,112,113,97;,
 4;111,114,115,112;,
 4;112,116,98,113;,
 4;112,115,110,116;;
 
 MeshMaterialList {
  2;
  78;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\NPC_human\\00_NPC.png";
   }
  }
  Material {
   0.633600;0.065600;0.125600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  108;
  0.833994;-0.062922;-0.548175;,
  0.492293;-0.042844;-0.869374;,
  0.462313;0.128379;0.877374;,
  0.973306;0.100988;0.206098;,
  0.834847;-0.101688;-0.541009;,
  0.520884;-0.027479;-0.853185;,
  0.462158;0.134945;0.876470;,
  0.977278;-0.032698;0.209424;,
  0.854844;-0.060233;-0.515378;,
  0.572137;0.047416;-0.818786;,
  0.470189;0.072179;0.879609;,
  0.977230;-0.091332;0.191519;,
  0.871903;0.064191;-0.485452;,
  0.599137;0.126910;-0.790524;,
  0.462542;0.067450;0.884028;,
  0.985333;0.034220;0.167176;,
  0.872811;0.108913;-0.475751;,
  0.604911;0.146972;-0.782612;,
  0.446838;0.132597;0.884734;,
  0.983315;0.085628;0.160499;,
  0.637495;0.082083;0.766070;,
  0.668309;0.003869;0.743874;,
  0.709959;0.067671;0.700984;,
  0.719697;0.129298;0.682142;,
  0.632634;0.161616;0.757400;,
  0.238302;0.124706;0.963151;,
  0.218359;0.148829;0.964453;,
  0.188838;0.066530;0.979752;,
  0.145223;-0.002692;0.989395;,
  0.112606;0.035403;0.993009;,
  -0.041201;0.032133;-0.998634;,
  -0.000373;0.102919;-0.994690;,
  0.013269;0.153276;-0.988094;,
  -0.057248;-0.069776;0.995919;,
  0.026522;0.126007;0.991675;,
  -0.025404;0.042641;-0.998767;,
  -0.881230;-0.041174;-0.470892;,
  -0.566893;-0.021892;-0.823501;,
  -0.335775;0.035962;0.941256;,
  -0.959109;0.064265;0.275646;,
  -0.871515;-0.065270;-0.486006;,
  -0.568592;-0.018486;-0.822412;,
  -0.371734;0.084843;0.924454;,
  -0.970948;-0.014962;0.238823;,
  -0.863407;-0.030687;-0.503574;,
  -0.576571;0.041671;-0.815984;,
  -0.428824;0.017193;0.903225;,
  -0.982015;-0.089045;0.166486;,
  -0.865757;0.019740;-0.500075;,
  -0.592818;0.085031;-0.800835;,
  -0.442736;-0.003011;0.896647;,
  -0.992447;-0.017416;0.121433;,
  -0.868657;0.011905;-0.495272;,
  -0.604461;0.071779;-0.793394;,
  -0.435718;0.094524;0.895106;,
  -0.992740;0.049410;0.109665;,
  -0.586504;0.046392;0.808617;,
  -0.660992;-0.054192;0.748434;,
  -0.719563;-0.012327;0.694317;,
  -0.736062;0.122776;0.665686;,
  -0.546112;0.052734;0.836051;,
  -0.165088;0.109142;0.980221;,
  -0.181225;0.118494;0.976277;,
  -0.211119;0.027741;0.977067;,
  -0.195038;-0.051641;0.979435;,
  -0.167318;-0.031026;0.985415;,
  0.017984;0.153638;-0.987963;,
  -0.028875;0.033408;0.999025;,
  0.007289;0.134062;0.990946;,
  -0.069045;-0.063909;0.995564;,
  -0.012350;0.994952;-0.099586;,
  -0.034096;0.999367;0.010160;,
  -0.092046;0.986462;-0.135719;,
  -0.082083;0.991979;-0.096121;,
  -0.003694;0.988271;-0.152665;,
  -0.045031;0.986843;-0.155283;,
  0.006108;0.999962;0.006229;,
  0.010124;0.999010;-0.043317;,
  0.008130;0.993201;-0.116130;,
  -0.006109;0.986310;-0.164788;,
  -0.021063;0.994518;-0.102421;,
  0.001159;0.994108;-0.108387;,
  0.017006;0.998667;-0.048733;,
  -0.041490;0.996736;-0.069249;,
  -0.035596;0.995277;-0.090310;,
  0.016622;0.999528;-0.025845;,
  -0.049853;0.993162;-0.105568;,
  0.030901;0.999468;-0.010459;,
  -0.009480;0.998265;-0.058109;,
  0.042150;-0.999076;-0.008373;,
  -0.035791;-0.999352;-0.003837;,
  0.053360;-0.998541;-0.008265;,
  -0.020763;-0.995044;-0.097239;,
  0.028457;-0.999086;-0.031899;,
  -0.028127;-0.995580;-0.089611;,
  -0.019807;-0.999516;-0.023977;,
  0.035517;-0.998353;-0.045052;,
  0.014926;-0.998211;-0.057905;,
  0.035393;-0.999198;-0.018736;,
  -0.035667;-0.998441;-0.042937;,
  -0.011623;-0.999141;-0.039782;,
  0.019178;-0.999471;-0.026249;,
  -0.016553;-0.996598;-0.080740;,
  0.068329;-0.997518;-0.017019;,
  0.053578;-0.997531;-0.045405;,
  0.040828;-0.998935;-0.021487;,
  0.081064;-0.995868;-0.040925;,
  0.060962;-0.997652;-0.031214;;
  78;
  4;4,5,1,0;,
  4;5,35,30,1;,
  4;34,68,26,25;,
  4;24,20,7,3;,
  4;7,4,0,3;,
  4;8,9,5,4;,
  4;9,31,35,5;,
  4;68,67,27,26;,
  4;6,10,21,20;,
  4;11,8,4,7;,
  4;12,13,9,8;,
  4;13,32,31,9;,
  4;67,33,28,27;,
  4;10,14,22,21;,
  4;15,12,8,11;,
  4;16,17,13,12;,
  4;17,66,32,13;,
  4;69,29,28,33;,
  4;14,18,23,22;,
  4;19,16,12,15;,
  4;20,21,11,7;,
  4;21,22,15,11;,
  4;22,23,19,15;,
  4;20,24,2,6;,
  4;26,6,2,25;,
  4;26,27,10,6;,
  4;27,28,14,10;,
  4;28,29,18,14;,
  4;37,41,40,36;,
  4;30,35,41,37;,
  4;62,68,34,61;,
  4;43,56,60,39;,
  4;36,40,43,39;,
  4;41,45,44,40;,
  4;35,31,45,41;,
  4;63,67,68,62;,
  4;57,46,42,56;,
  4;40,44,47,43;,
  4;45,49,48,44;,
  4;31,32,49,45;,
  4;64,33,67,63;,
  4;58,50,46,57;,
  4;44,48,51,47;,
  4;49,53,52,48;,
  4;32,66,53,49;,
  4;64,65,69,33;,
  4;59,54,50,58;,
  4;48,52,55,51;,
  4;47,57,56,43;,
  4;51,58,57,47;,
  4;55,59,58,51;,
  4;38,60,56,42;,
  4;38,42,62,61;,
  4;46,63,62,42;,
  4;50,64,63,46;,
  4;54,65,64,50;,
  4;79,81,78,74;,
  4;81,76,71,80;,
  4;81,80,75,78;,
  4;82,76,81,82;,
  4;82,82,79,70;,
  4;83,75,73,83;,
  4;83,83,77,78;,
  4;86,74,85,88;,
  4;86,88,84,72;,
  4;88,85,78,87;,
  4;88,87,77,84;,
  4;98,93,97,100;,
  4;100,99,90,95;,
  4;100,97,94,99;,
  4;101,101,100,95;,
  4;101,89,98,101;,
  4;102,102,92,94;,
  4;102,97,96,102;,
  4;105,107,104,93;,
  4;105,91,103,107;,
  4;107,106,97,104;,
  4;107,103,96,106;;
 }
 MeshTextureCoords {
  117;
  0.225280;0.841140;,
  0.180450;0.842380;,
  0.183960;0.806370;,
  0.226170;0.810290;,
  0.137030;0.844220;,
  0.136790;0.802970;,
  0.382080;0.790720;,
  0.382080;0.835900;,
  0.362050;0.836670;,
  0.362410;0.790600;,
  0.297710;0.800980;,
  0.299530;0.837670;,
  0.271040;0.839090;,
  0.271480;0.809890;,
  0.225220;0.872160;,
  0.180780;0.873630;,
  0.137210;0.875700;,
  0.382080;0.869230;,
  0.361630;0.871080;,
  0.332100;0.836880;,
  0.331560;0.870260;,
  0.299790;0.870530;,
  0.269530;0.870890;,
  0.226210;0.901530;,
  0.182900;0.902900;,
  0.137240;0.905110;,
  0.382080;0.898770;,
  0.360710;0.899810;,
  0.331100;0.900100;,
  0.298290;0.900240;,
  0.269530;0.900300;,
  0.226320;0.932650;,
  0.183770;0.933940;,
  0.137220;0.935880;,
  0.381960;0.931320;,
  0.359340;0.931960;,
  0.329030;0.932630;,
  0.297520;0.932650;,
  0.269990;0.931950;,
  0.331250;0.792810;,
  0.089530;0.804760;,
  0.093620;0.841620;,
  0.049400;0.840090;,
  0.047880;0.807060;,
  0.401760;0.835410;,
  0.401530;0.789280;,
  0.492780;0.838820;,
  0.464000;0.837360;,
  0.464970;0.801690;,
  0.492200;0.810390;,
  0.004140;0.838820;,
  0.003560;0.810390;,
  0.093700;0.873340;,
  0.049520;0.871830;,
  0.402190;0.869860;,
  0.463870;0.869770;,
  0.432120;0.869230;,
  0.431460;0.836030;,
  0.005470;0.870420;,
  0.091610;0.902810;,
  0.048490;0.901450;,
  0.403070;0.898630;,
  0.465360;0.899360;,
  0.432600;0.899040;,
  0.005330;0.899850;,
  0.090710;0.934080;,
  0.048260;0.932800;,
  0.404550;0.930860;,
  0.466080;0.931800;,
  0.434740;0.931580;,
  0.004970;0.931640;,
  0.494100;0.870420;,
  0.493960;0.899850;,
  0.493610;0.931640;,
  0.432050;0.792300;,
  0.000000;0.326340;,
  0.535740;0.291300;,
  0.537740;0.518280;,
  0.000000;0.543670;,
  0.533170;0.000000;,
  1.000000;0.000000;,
  1.000000;0.212850;,
  1.000000;0.496450;,
  0.265700;0.000000;,
  0.294750;0.307060;,
  0.000000;0.000000;,
  0.763370;0.507620;,
  1.000000;1.000000;,
  0.754370;1.000000;,
  0.553230;1.000000;,
  0.000000;0.745380;,
  0.285370;0.530190;,
  0.274640;0.745400;,
  0.261940;1.000000;,
  0.000000;1.000000;,
  0.545050;0.745590;,
  0.000000;0.326340;,
  0.000000;0.543670;,
  0.537740;0.518280;,
  0.535740;0.291300;,
  1.000000;0.212850;,
  1.000000;0.000000;,
  0.533170;0.000000;,
  1.000000;0.496450;,
  0.265700;0.000000;,
  0.294750;0.307060;,
  0.000000;0.000000;,
  0.763370;0.507620;,
  0.754370;1.000000;,
  1.000000;1.000000;,
  0.553230;1.000000;,
  0.000000;0.745380;,
  0.274640;0.745400;,
  0.285370;0.530190;,
  0.000000;1.000000;,
  0.261940;1.000000;,
  0.545050;0.745590;;
 }
}
