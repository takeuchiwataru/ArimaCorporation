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
 -0.96236;-0.09021;-1.09276;,
 -1.80312;-0.21974;-0.14482;,
 -0.09502;3.01550;-0.77105;,
 -0.34906;2.74843;2.24540;,
 1.37033;5.83419;1.75387;,
 0.59062;5.66603;2.48481;,
 -1.23905;-0.20224;0.67826;,
 -0.52522;-0.23658;1.31714;,
 -1.04077;2.70323;1.79397;,
 0.59062;5.66603;2.48481;,
 -0.31595;5.59767;3.20160;,
 -0.34906;2.74843;2.24540;,
 -1.80312;-0.21974;-0.14482;,
 -1.37919;8.31183;2.95648;,
 -0.73312;5.93557;1.24207;,
 1.65569;8.39601;2.61026;,
 1.37033;5.83419;1.75387;,
 -0.06091;6.03266;0.39195;,
 0.41706;0.04970;-1.22071;,
 -0.18293;0.11975;-1.98738;,
 -0.09502;3.01550;-0.77105;,
 -0.34906;2.74843;2.24540;,
 -0.31595;5.59767;3.20160;,
 -0.91366;5.58066;2.66981;,
 0.03171;8.68075;0.96605;,
 -0.06091;6.03266;0.39195;,
 -0.83072;8.55763;1.97663;,
 1.92733;10.92500;3.66360;,
 -0.06091;6.03266;0.39195;,
 0.03171;8.68075;0.96605;,
 -0.17142;8.12819;4.46334;,
 -0.31595;5.59767;3.20160;,
 0.59062;5.66603;2.48481;,
 -0.76756;10.82230;4.74551;,
 -0.52300;9.08436;4.23678;,
 -0.17142;8.12819;4.46334;,
 -0.73312;5.93557;1.24207;,
 -1.37919;8.31183;2.95648;,
 -0.83072;8.55763;1.97663;,
 0.03171;8.68075;0.96605;,
 1.01323;10.99285;2.64298;,
 1.92733;10.92500;3.66360;,
 -0.67545;11.01931;2.60840;,
 0.10384;11.28315;1.58393;,
 0.03171;8.68075;0.96605;,
 -1.80312;-0.21974;-0.14482;,
 -0.06091;6.03266;0.39195;,
 -0.09502;3.01550;-0.77105;,
 1.18073;2.86575;0.59754;,
 0.51901;2.97433;-0.05111;,
 -0.06091;6.03266;0.39195;,
 1.18073;2.86575;0.59754;,
 1.13830;0.02818;-0.36352;,
 0.51901;2.97433;-0.05111;,
 -0.52522;-0.23658;1.31714;,
 1.13830;0.02818;-0.36352;,
 1.18073;2.86575;0.59754;,
 -0.91366;5.58066;2.66981;,
 -0.31595;5.59767;3.20160;,
 -0.89497;8.22062;3.57245;,
 -0.89497;8.22062;3.57245;,
 -0.31595;5.59767;3.20160;,
 -0.17142;8.12819;4.46334;,
 -0.89497;8.22062;3.57245;,
 -1.33421;10.94929;3.80902;,
 -1.37919;8.31183;2.95648;,
 -1.37919;8.31183;2.95648;,
 -1.33421;10.94929;3.80902;,
 -0.83072;8.55763;1.97663;,
 0.59062;5.66603;2.48481;,
 1.37033;5.83419;1.75387;,
 1.65569;8.39601;2.61026;,
 0.59062;5.66603;2.48481;,
 1.92733;10.92500;3.66360;,
 0.88590;10.85946;4.67484;,
 0.51901;2.97433;-0.05111;,
 1.13830;0.02818;-0.36352;,
 0.41706;0.04970;-1.22071;,
 -0.83072;8.55763;1.97663;,
 -1.33421;10.94929;3.80902;,
 -0.67545;11.01931;2.60840;,
 -0.67545;11.01931;2.60840;,
 -0.39128;9.41937;1.92105;,
 -0.83072;8.55763;1.97663;,
 -0.76756;10.82230;4.74551;,
 -1.33421;10.94929;3.80902;,
 -0.89497;8.22062;3.57245;,
 -0.17142;8.12819;4.46334;,
 -0.07209;10.61046;5.61820;,
 -0.76756;10.82230;4.74551;,
 -0.17142;8.12819;4.46334;,
 0.59062;5.66603;2.48481;,
 0.88590;10.85946;4.67484;,
 -0.91366;5.58066;2.66981;,
 -0.89497;8.22062;3.57245;,
 -1.37919;8.31183;2.95648;,
 -1.04077;2.70323;1.79397;,
 -1.37919;8.31183;2.95648;,
 -1.80312;-0.21974;-0.14482;,
 -0.09502;3.01550;-0.77105;,
 -0.06091;6.03266;0.39195;,
 0.51901;2.97433;-0.05111;,
 -0.09502;3.01550;-0.77105;,
 0.35156;2.13494;-0.69217;,
 0.41706;0.04970;-1.22071;,
 0.03171;8.68075;0.96605;,
 -0.39128;9.41937;1.92105;,
 -0.67545;11.01931;2.60840;,
 -0.83072;8.55763;1.97663;,
 -0.39128;9.41937;1.92105;,
 0.03171;8.68075;0.96605;,
 -1.80312;-0.21974;-0.14482;,
 -1.23905;-0.20224;0.67826;,
 -1.04077;2.70323;1.79397;,
 -0.09502;3.01550;-0.77105;,
 -0.18293;0.11975;-1.98738;,
 -0.96236;-0.09021;-1.09276;,
 -1.04077;2.70323;1.79397;,
 -0.52522;-0.23658;1.31714;,
 -0.34906;2.74843;2.24540;,
 -0.34906;2.74843;2.24540;,
 -0.52522;-0.23658;1.31714;,
 1.18073;2.86575;0.59754;,
 1.65569;8.39601;2.61026;,
 1.92733;10.92500;3.66360;,
 0.59062;5.66603;2.48481;,
 1.92733;10.92500;3.66360;,
 1.65569;8.39601;2.61026;,
 -0.06091;6.03266;0.39195;,
 -0.06091;6.03266;0.39195;,
 1.37033;5.83419;1.75387;,
 1.18073;2.86575;0.59754;,
 1.18073;2.86575;0.59754;,
 1.37033;5.83419;1.75387;,
 -0.34906;2.74843;2.24540;,
 0.51901;2.97433;-0.05111;,
 0.35156;2.13494;-0.69217;,
 -0.09502;3.01550;-0.77105;,
 0.41706;0.04970;-1.22071;,
 0.35156;2.13494;-0.69217;,
 0.51901;2.97433;-0.05111;,
 0.03171;8.68075;0.96605;,
 0.10384;11.28315;1.58393;,
 1.01323;10.99285;2.64298;,
 -0.73312;5.93557;1.24207;,
 -0.06091;6.03266;0.39195;,
 -1.80312;-0.21974;-0.14482;,
 -0.83072;8.55763;1.97663;,
 -0.06091;6.03266;0.39195;,
 -0.73312;5.93557;1.24207;,
 -0.17142;8.12819;4.46334;,
 -0.52300;9.08436;4.23678;,
 -0.89497;8.22062;3.57245;,
 -0.89497;8.22062;3.57245;,
 -0.52300;9.08436;4.23678;,
 -0.76756;10.82230;4.74551;,
 0.88590;10.85946;4.67484;,
 -0.07209;10.61046;5.61820;,
 -0.17142;8.12819;4.46334;,
 -0.91366;5.58066;2.66981;,
 -1.04077;2.70323;1.79397;,
 -0.34906;2.74843;2.24540;,
 -0.91366;5.58066;2.66981;,
 -1.37919;8.31183;2.95648;,
 -1.04077;2.70323;1.79397;;
 
 66;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,16,17;,
 3;18,19,20;,
 3;21,22,23;,
 3;24,25,26;,
 3;27,28,29;,
 3;30,31,32;,
 3;33,34,35;,
 3;36,37,38;,
 3;39,40,41;,
 3;42,43,44;,
 3;45,46,47;,
 3;48,49,50;,
 3;51,52,53;,
 3;54,55,56;,
 3;57,58,59;,
 3;60,61,62;,
 3;63,64,65;,
 3;66,67,68;,
 3;69,70,71;,
 3;72,73,74;,
 3;75,76,77;,
 3;78,79,80;,
 3;81,82,83;,
 3;84,85,86;,
 3;87,88,89;,
 3;90,91,92;,
 3;93,94,95;,
 3;96,97,98;,
 3;99,100,101;,
 3;102,103,104;,
 3;105,106,107;,
 3;108,109,110;,
 3;111,112,113;,
 3;114,115,116;,
 3;117,118,119;,
 3;120,121,122;,
 3;123,124,125;,
 3;126,127,128;,
 3;129,130,131;,
 3;132,133,134;,
 3;135,136,137;,
 3;138,139,140;,
 3;141,142,143;,
 3;144,145,146;,
 3;147,148,149;,
 3;150,151,152;,
 3;153,154,155;,
 3;156,157,158;,
 3;159,160,161;,
 3;162,163,164;,
 3;41,143,74;,
 3;143,43,74;,
 3;74,43,157;,
 3;43,80,157;,
 3;157,80,89;,
 3;80,85,89;,
 3;55,7,77;,
 3;77,7,19;,
 3;19,7,116;,
 3;7,112,116;,
 3;112,1,116;;
 
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "�j���g��.jpg";
   }
  }
 }
 MeshNormals {
  68;
  -0.820731;-0.179147;0.542501;,
  0.725304;0.221577;-0.651796;,
  0.714646;0.171011;-0.678260;,
  -0.805090;0.158052;-0.571708;,
  -0.590657;-0.229077;0.773724;,
  -0.737156;0.265285;-0.621470;,
  -0.787224;-0.233951;0.570566;,
  0.705545;0.200188;-0.679802;,
  0.777409;0.223379;-0.587994;,
  0.601771;-0.323418;0.730255;,
  -0.805300;0.150323;-0.573494;,
  -0.812329;0.170965;-0.557577;,
  -0.738827;0.266495;-0.618963;,
  0.696413;0.224966;-0.681469;,
  -0.775017;-0.230327;0.588471;,
  -0.832021;0.177810;-0.525476;,
  0.671192;-0.311971;0.672440;,
  0.763077;0.202845;-0.613651;,
  -0.793039;0.190139;-0.578737;,
  -0.797722;-0.241555;0.552531;,
  0.661451;-0.342461;0.667235;,
  -0.655839;-0.220194;0.722074;,
  0.753331;0.227881;-0.616898;,
  -0.760736;0.203895;-0.616205;,
  -0.738955;-0.196989;0.644314;,
  -0.747340;0.271531;-0.606427;,
  0.683718;-0.269004;0.678356;,
  0.659936;0.200341;-0.724119;,
  0.695250;-0.289791;0.657760;,
  0.761100;0.227433;-0.607454;,
  0.755315;0.143195;-0.639526;,
  -0.792926;0.194394;-0.577476;,
  -0.750633;-0.214474;0.624941;,
  0.686935;-0.319508;0.652713;,
  -0.673154;-0.202516;0.711232;,
  0.690381;0.191722;-0.697580;,
  0.711587;0.249379;-0.656852;,
  -0.563430;-0.230415;0.793382;,
  -0.684655;-0.245628;0.686232;,
  -0.760788;0.206381;-0.615312;,
  0.759617;0.132134;-0.636807;,
  -0.810247;-0.217328;0.544305;,
  0.714150;-0.240199;0.657491;,
  0.709005;-0.235711;0.664645;,
  0.713286;-0.258319;0.651532;,
  -0.763970;-0.182892;0.618789;,
  -0.872769;0.167535;-0.458482;,
  0.720857;-0.309726;0.620028;,
  0.713093;-0.320960;0.623285;,
  -0.809470;-0.163026;0.564075;,
  -0.756179;0.272794;-0.594792;,
  0.769483;0.127268;-0.625858;,
  0.705452;-0.321985;0.631398;,
  0.000867;0.997848;0.065561;,
  0.111120;0.991193;0.072034;,
  0.041282;0.993938;0.101898;,
  -0.000766;0.991058;0.133433;,
  -0.015723;0.990057;0.139788;,
  0.039679;0.992255;0.117708;,
  0.121233;0.987942;0.096301;,
  0.066532;0.993305;0.094438;,
  0.071858;-0.993751;-0.085418;,
  0.065179;-0.994193;-0.085630;,
  0.045185;-0.994413;-0.095397;,
  0.090673;-0.991099;-0.097477;,
  0.090089;-0.993726;-0.066278;,
  0.053700;-0.997059;-0.054687;,
  0.099817;-0.993882;-0.047281;;
  66;
  3;25,5,12;,
  3;26,28,16;,
  3;24,4,34;,
  3;16,9,26;,
  3;5,15,31;,
  3;27,35,7;,
  3;17,8,36;,
  3;37,38,21;,
  3;11,39,18;,
  3;2,7,40;,
  3;20,9,16;,
  3;19,14,41;,
  3;31,15,18;,
  3;40,30,2;,
  3;10,3,11;,
  3;5,39,12;,
  3;13,29,7;,
  3;13,1,29;,
  3;42,43,44;,
  3;21,38,32;,
  3;32,38,41;,
  3;32,0,45;,
  3;15,46,18;,
  3;16,28,47;,
  3;16,48,33;,
  3;29,1,17;,
  3;18,46,10;,
  3;10,23,18;,
  3;19,0,32;,
  3;41,6,19;,
  3;20,16,33;,
  3;21,32,45;,
  3;34,45,49;,
  3;36,7,29;,
  3;36,22,17;,
  3;11,23,10;,
  3;18,23,11;,
  3;49,24,34;,
  3;12,50,25;,
  3;34,4,37;,
  3;26,42,44;,
  3;47,48,16;,
  3;2,27,7;,
  3;7,35,13;,
  3;44,28,26;,
  3;29,22,36;,
  3;17,22,29;,
  3;40,51,30;,
  3;31,39,5;,
  3;18,39,31;,
  3;41,14,32;,
  3;32,14,19;,
  3;33,52,20;,
  3;21,34,37;,
  3;21,45,34;,
  3;53,54,55;,
  3;54,56,55;,
  3;55,56,57;,
  3;56,58,57;,
  3;57,58,59;,
  3;58,60,59;,
  3;61,62,63;,
  3;63,62,64;,
  3;64,62,65;,
  3;62,66,65;,
  3;66,67,65;;
 }
 MeshTextureCoords {
  165;
  0.171116;0.815245;,
  0.171116;0.815245;,
  0.171116;0.815245;,
  0.142798;0.667024;,
  0.142798;0.667024;,
  0.142798;0.667024;,
  0.134790;0.567399;,
  0.134790;0.567399;,
  0.134790;0.567399;,
  0.157708;0.730197;,
  0.157708;0.730197;,
  0.157708;0.730197;,
  0.053446;0.757374;,
  0.053446;0.757374;,
  0.053446;0.757374;,
  0.141680;0.750860;,
  0.141680;0.750860;,
  0.141680;0.750860;,
  0.145768;0.665294;,
  0.145768;0.665294;,
  0.145768;0.665294;,
  0.143128;0.583016;,
  0.143128;0.583016;,
  0.143128;0.583016;,
  0.128263;0.595470;,
  0.128263;0.595470;,
  0.128263;0.595470;,
  0.092123;0.559543;,
  0.092123;0.559543;,
  0.092123;0.559543;,
  0.126334;0.533816;,
  0.126334;0.533816;,
  0.126334;0.533816;,
  0.125483;0.617459;,
  0.125483;0.617459;,
  0.125483;0.617459;,
  0.126821;0.524266;,
  0.126821;0.524266;,
  0.126821;0.524266;,
  0.138646;0.655707;,
  0.138646;0.655707;,
  0.138646;0.655707;,
  0.167597;0.766010;,
  0.167597;0.766010;,
  0.167597;0.766010;,
  0.048699;0.596279;,
  0.048699;0.596279;,
  0.048699;0.596279;,
  0.128400;0.535833;,
  0.128400;0.535833;,
  0.128400;0.535833;,
  0.167216;0.727238;,
  0.167216;0.727238;,
  0.167216;0.727238;,
  0.162112;0.890244;,
  0.162112;0.890244;,
  0.162112;0.890244;,
  0.144098;0.579197;,
  0.144098;0.579197;,
  0.144098;0.579197;,
  0.158184;0.716866;,
  0.158184;0.716866;,
  0.158184;0.716866;,
  0.128251;0.890022;,
  0.128251;0.890022;,
  0.128251;0.890022;,
  0.139664;0.652272;,
  0.139664;0.652272;,
  0.139664;0.652272;,
  0.123311;0.890609;,
  0.123311;0.890609;,
  0.123311;0.890609;,
  0.104795;0.621486;,
  0.104795;0.621486;,
  0.104795;0.621486;,
  0.161046;0.740752;,
  0.161046;0.740752;,
  0.161046;0.740752;,
  0.126909;0.554350;,
  0.126909;0.554350;,
  0.126909;0.554350;,
  0.158025;0.584871;,
  0.158025;0.584871;,
  0.158025;0.584871;,
  0.140160;0.632784;,
  0.140160;0.632784;,
  0.140160;0.632784;,
  0.156485;0.709955;,
  0.156485;0.709955;,
  0.156485;0.709955;,
  0.105319;0.606849;,
  0.105319;0.606849;,
  0.105319;0.606849;,
  0.169415;0.710185;,
  0.169415;0.710185;,
  0.169415;0.710185;,
  0.081735;0.552644;,
  0.081735;0.552644;,
  0.081735;0.552644;,
  0.062376;0.730352;,
  0.062376;0.730352;,
  0.062376;0.730352;,
  0.125633;0.695419;,
  0.125633;0.695419;,
  0.125633;0.695419;,
  0.123376;0.541450;,
  0.123376;0.541450;,
  0.123376;0.541450;,
  0.164792;0.617448;,
  0.164792;0.617448;,
  0.164792;0.617448;,
  0.131607;0.560975;,
  0.131607;0.560975;,
  0.131607;0.560975;,
  0.157991;0.750709;,
  0.157991;0.750709;,
  0.157991;0.750709;,
  0.108268;0.652177;,
  0.108268;0.652177;,
  0.108268;0.652177;,
  0.133763;0.762656;,
  0.133763;0.762656;,
  0.133763;0.762656;,
  0.076923;0.659605;,
  0.076923;0.659605;,
  0.076923;0.659605;,
  0.072401;0.663871;,
  0.072401;0.663871;,
  0.072401;0.663871;,
  0.129963;0.726939;,
  0.129963;0.726939;,
  0.129963;0.726939;,
  0.123904;0.726929;,
  0.123904;0.726929;,
  0.123904;0.726929;,
  0.042621;0.645670;,
  0.042621;0.645670;,
  0.042621;0.645670;,
  0.049177;0.516626;,
  0.049177;0.516626;,
  0.049177;0.516626;,
  0.149536;0.716821;,
  0.149536;0.716821;,
  0.149536;0.716821;,
  0.099834;0.691608;,
  0.099834;0.691608;,
  0.099834;0.691608;,
  0.136888;0.601907;,
  0.136888;0.601907;,
  0.136888;0.601907;,
  0.166786;0.758860;,
  0.166786;0.758860;,
  0.166786;0.758860;,
  0.111649;0.574145;,
  0.111649;0.574145;,
  0.111649;0.574145;,
  0.146886;0.707652;,
  0.146886;0.707652;,
  0.146886;0.707652;,
  0.108573;0.658168;,
  0.108573;0.658168;,
  0.108573;0.658168;,
  0.054139;0.784196;,
  0.054139;0.784196;,
  0.054139;0.784196;;
 }
}
