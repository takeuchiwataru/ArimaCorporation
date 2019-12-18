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
 352;
 1.94438;-15.94961;-1.91454;,
 1.59824;-15.98451;-1.67119;,
 2.19622;-14.90731;-1.78421;,
 1.87933;-14.95932;-0.82350;,
 2.47069;-13.93015;-0.89003;,
 2.16128;-13.98039;-0.71170;,
 1.71693;-15.96580;-1.36100;,
 1.89858;-15.96602;-1.09812;,
 1.69037;-14.98314;-1.02289;,
 2.16128;-13.98039;-0.71170;,
 1.81173;-13.99813;-0.54915;,
 1.87933;-14.95932;-0.82350;,
 1.59824;-15.98451;-1.67119;,
 1.48254;-13.10194;-0.74732;,
 1.82867;-13.91216;-1.22070;,
 2.49300;-13.06483;-0.62456;,
 2.47069;-13.93015;-0.89003;,
 2.11261;-13.88757;-1.44504;,
 2.40123;-15.89851;-1.85057;,
 2.26638;-15.88752;-2.14645;,
 2.19622;-14.90731;-1.78421;,
 1.87933;-14.95932;-0.82350;,
 1.81173;-13.99813;-0.54915;,
 1.65945;-14.01315;-0.76763;,
 2.09432;-12.99793;-1.28713;,
 2.11261;-13.88757;-1.44504;,
 1.73627;-13.03001;-1.02527;,
 2.49558;-12.20802;-0.29594;,
 2.11261;-13.88757;-1.44504;,
 2.09432;-12.99793;-1.28713;,
 1.75688;-13.13875;-0.16294;,
 1.81173;-13.99813;-0.54915;,
 2.16128;-13.98039;-0.71170;,
 1.53803;-12.24051;-0.15375;,
 1.65927;-12.82469;-0.27637;,
 1.75688;-13.13875;-0.16294;,
 1.82867;-13.91216;-1.22070;,
 1.48254;-13.10194;-0.74732;,
 1.73627;-13.03001;-1.02527;,
 2.09432;-12.99793;-1.28713;,
 2.27871;-12.20235;-0.69847;,
 2.49558;-12.20802;-0.29594;,
 1.73405;-12.20145;-0.84134;,
 2.06605;-12.12305;-1.11604;,
 2.09432;-12.99793;-1.28713;,
 1.59824;-15.98451;-1.67119;,
 2.11261;-13.88757;-1.44504;,
 2.19622;-14.90731;-1.78421;,
 2.50332;-14.93427;-1.23978;,
 2.33922;-14.90922;-1.50276;,
 2.11261;-13.88757;-1.44504;,
 2.50332;-14.93427;-1.23978;,
 2.56827;-15.89168;-1.51658;,
 2.33922;-14.90922;-1.50276;,
 1.89858;-15.96602;-1.09812;,
 2.56827;-15.89168;-1.51658;,
 2.50332;-14.93427;-1.23978;,
 1.65945;-14.01315;-0.76763;,
 1.81173;-13.99813;-0.54915;,
 1.59163;-13.12240;-0.50896;,
 1.59163;-13.12240;-0.50896;,
 1.81173;-13.99813;-0.54915;,
 1.75688;-13.13875;-0.16294;,
 1.59163;-13.12240;-0.50896;,
 1.42714;-12.21254;-0.50280;,
 1.48254;-13.10194;-0.74732;,
 1.48254;-13.10194;-0.74732;,
 1.42714;-12.21254;-0.50280;,
 1.73627;-13.03001;-1.02527;,
 2.16128;-13.98039;-0.71170;,
 2.47069;-13.93015;-0.89003;,
 2.49300;-13.06483;-0.62456;,
 2.16128;-13.98039;-0.71170;,
 2.49558;-12.20802;-0.29594;,
 2.07938;-12.22170;-0.04855;,
 2.33922;-14.90922;-1.50276;,
 2.56827;-15.89168;-1.51658;,
 2.40123;-15.89851;-1.85057;,
 1.73627;-13.03001;-1.02527;,
 1.42714;-12.21254;-0.50280;,
 1.73405;-12.20145;-0.84134;,
 1.73405;-12.20145;-0.84134;,
 1.88185;-12.74175;-1.02056;,
 1.73627;-13.03001;-1.02527;,
 1.53803;-12.24051;-0.15375;,
 1.42714;-12.21254;-0.50280;,
 1.59163;-13.12240;-0.50896;,
 1.75688;-13.13875;-0.16294;,
 1.69576;-12.29697;0.18578;,
 1.53803;-12.24051;-0.15375;,
 1.75688;-13.13875;-0.16294;,
 2.16128;-13.98039;-0.71170;,
 2.07938;-12.22170;-0.04855;,
 1.65945;-14.01315;-0.76763;,
 1.59163;-13.12240;-0.50896;,
 1.48254;-13.10194;-0.74732;,
 1.69037;-14.98314;-1.02289;,
 1.48254;-13.10194;-0.74732;,
 1.59824;-15.98451;-1.67119;,
 2.19622;-14.90731;-1.78421;,
 2.11261;-13.88757;-1.44504;,
 2.33922;-14.90922;-1.50276;,
 2.19622;-14.90731;-1.78421;,
 2.33601;-15.19762;-1.71223;,
 2.40123;-15.89851;-1.85057;,
 2.09432;-12.99793;-1.28713;,
 1.88185;-12.74175;-1.02056;,
 1.73405;-12.20145;-0.84134;,
 1.73627;-13.03001;-1.02527;,
 1.88185;-12.74175;-1.02056;,
 2.09432;-12.99793;-1.28713;,
 1.59824;-15.98451;-1.67119;,
 1.71693;-15.96580;-1.36100;,
 1.69037;-14.98314;-1.02289;,
 2.19622;-14.90731;-1.78421;,
 2.26638;-15.88752;-2.14645;,
 1.94438;-15.94961;-1.91454;,
 1.69037;-14.98314;-1.02289;,
 1.89858;-15.96602;-1.09812;,
 1.87933;-14.95932;-0.82350;,
 1.87933;-14.95932;-0.82350;,
 1.89858;-15.96602;-1.09812;,
 2.50332;-14.93427;-1.23978;,
 2.49300;-13.06483;-0.62456;,
 2.49558;-12.20802;-0.29594;,
 2.16128;-13.98039;-0.71170;,
 2.49558;-12.20802;-0.29594;,
 2.49300;-13.06483;-0.62456;,
 2.11261;-13.88757;-1.44504;,
 2.11261;-13.88757;-1.44504;,
 2.47069;-13.93015;-0.89003;,
 2.50332;-14.93427;-1.23978;,
 2.50332;-14.93427;-1.23978;,
 2.47069;-13.93015;-0.89003;,
 1.87933;-14.95932;-0.82350;,
 2.33922;-14.90922;-1.50276;,
 2.33601;-15.19762;-1.71223;,
 2.19622;-14.90731;-1.78421;,
 2.40123;-15.89851;-1.85057;,
 2.33601;-15.19762;-1.71223;,
 2.33922;-14.90922;-1.50276;,
 2.09432;-12.99793;-1.28713;,
 2.06605;-12.12305;-1.11604;,
 2.27871;-12.20235;-0.69847;,
 1.82867;-13.91216;-1.22070;,
 2.11261;-13.88757;-1.44504;,
 1.59824;-15.98451;-1.67119;,
 1.73627;-13.03001;-1.02527;,
 2.11261;-13.88757;-1.44504;,
 1.82867;-13.91216;-1.22070;,
 1.75688;-13.13875;-0.16294;,
 1.65927;-12.82469;-0.27637;,
 1.59163;-13.12240;-0.50896;,
 1.59163;-13.12240;-0.50896;,
 1.65927;-12.82469;-0.27637;,
 1.53803;-12.24051;-0.15375;,
 2.07938;-12.22170;-0.04855;,
 1.69576;-12.29697;0.18578;,
 1.75688;-13.13875;-0.16294;,
 1.65945;-14.01315;-0.76763;,
 1.69037;-14.98314;-1.02289;,
 1.87933;-14.95932;-0.82350;,
 1.65945;-14.01315;-0.76763;,
 1.48254;-13.10194;-0.74732;,
 1.69037;-14.98314;-1.02289;,
 4.30299;-4.77029;-1.14905;,
 3.83251;-5.14286;1.93323;,
 3.73144;-9.16313;0.62800;,
 0.53422;-11.55259;-0.42671;,
 1.66024;-11.00806;1.14266;,
 -0.24469;-10.69765;-0.59040;,
 4.30299;-4.77029;-1.14905;,
 3.73144;-9.16313;0.62800;,
 3.94849;-9.01234;-0.69217;,
 0.28069;-10.15331;-2.41045;,
 0.53422;-11.55259;-0.42671;,
 -0.24469;-10.69765;-0.59040;,
 0.90119;-10.26900;1.51142;,
 3.73144;-9.16313;0.62800;,
 1.73123;-5.46399;4.70198;,
 3.63762;-4.89899;-3.41964;,
 3.23432;-8.41202;-3.38292;,
 2.79270;-5.54002;-4.00372;,
 1.50301;-11.12757;-2.45144;,
 1.97381;-9.47915;-3.51521;,
 3.43714;-10.66305;-1.94332;,
 1.97427;-12.33151;-0.44598;,
 1.16753;-12.46164;-0.44193;,
 2.10980;-12.29277;-1.45276;,
 3.63762;-4.89899;-3.41964;,
 4.30299;-4.77029;-1.14905;,
 4.08840;-8.84392;-2.04558;,
 1.96368;-6.09266;-4.60398;,
 1.16673;-9.21349;-3.58722;,
 -1.03834;-7.05504;-4.51551;,
 1.50301;-11.12757;-2.45144;,
 0.28069;-10.15331;-2.41045;,
 1.97381;-9.47915;-3.51521;,
 1.96368;-6.09266;-4.60398;,
 3.23432;-8.41202;-3.38292;,
 1.97381;-9.47915;-3.51521;,
 1.16673;-9.21349;-3.58722;,
 1.96368;-6.09266;-4.60398;,
 1.97381;-9.47915;-3.51521;,
 3.43714;-10.66305;-1.94332;,
 1.97381;-9.47915;-3.51521;,
 4.08840;-8.84392;-2.04558;,
 3.55958;-10.07961;-1.26825;,
 3.43714;-10.66305;-1.94332;,
 4.08840;-8.84392;-2.04558;,
 1.16753;-12.46164;-0.44193;,
 1.50301;-11.12757;-2.45144;,
 2.10980;-12.29277;-1.45276;,
 1.50301;-11.12757;-2.45144;,
 3.43714;-10.66305;-1.94332;,
 2.10980;-12.29277;-1.45276;,
 3.20361;-10.78615;0.19549;,
 2.28072;-12.49104;0.07501;,
 2.68341;-12.47692;-0.24219;,
 1.66024;-11.00806;1.14266;,
 3.73144;-9.16313;0.62800;,
 0.90119;-10.26900;1.51142;,
 1.66024;-11.00806;1.14266;,
 0.35667;-10.40256;0.33275;,
 -0.24469;-10.69765;-0.59040;,
 0.84869;-12.01052;-0.46271;,
 1.86431;-12.50982;0.38607;,
 0.53422;-11.55259;-0.42671;,
 3.20361;-10.78615;0.19549;,
 3.94849;-9.01234;-0.69217;,
 3.73144;-9.16313;0.62800;,
 0.90119;-10.26900;1.51142;,
 -1.05323;-7.83762;-0.68747;,
 0.35667;-10.40256;0.33275;,
 3.20361;-10.78615;0.19549;,
 1.66024;-11.00806;1.14266;,
 2.28072;-12.49104;0.07501;,
 3.23432;-8.41202;-3.38292;,
 3.63762;-4.89899;-3.41964;,
 4.08840;-8.84392;-2.04558;,
 1.97381;-9.47915;-3.51521;,
 3.23432;-8.41202;-3.38292;,
 4.08840;-8.84392;-2.04558;,
 2.79270;-5.54002;-4.00372;,
 3.23432;-8.41202;-3.38292;,
 1.96368;-6.09266;-4.60398;,
 -1.05323;-7.83762;-0.68747;,
 0.90119;-10.26900;1.51142;,
 -1.00171;-7.80018;0.78893;,
 1.66024;-11.00806;1.14266;,
 3.20361;-10.78615;0.19549;,
 3.73144;-9.16313;0.62800;,
 2.28072;-12.49104;0.07501;,
 1.66024;-11.00806;1.14266;,
 1.86431;-12.50982;0.38607;,
 0.35667;-10.40256;0.33275;,
 1.66024;-11.00806;1.14266;,
 0.90119;-10.26900;1.51142;,
 3.55958;-10.07961;-1.26825;,
 3.20361;-10.78615;0.19549;,
 2.68341;-12.47692;-0.24219;,
 -1.05323;-7.83762;-0.68747;,
 -1.03834;-7.05504;-4.51551;,
 -0.24469;-10.69765;-0.59040;,
 3.94849;-9.01234;-0.69217;,
 3.55958;-10.07961;-1.26825;,
 4.08840;-8.84392;-2.04558;,
 4.30299;-4.77029;-1.14905;,
 3.94849;-9.01234;-0.69217;,
 4.08840;-8.84392;-2.04558;,
 1.73123;-5.46399;4.70198;,
 3.73144;-9.16313;0.62800;,
 3.83251;-5.14286;1.93323;,
 2.28072;-12.49104;0.07501;,
 1.16753;-12.46164;-0.44193;,
 1.97427;-12.33151;-0.44598;,
 2.68341;-12.47692;-0.24219;,
 2.28072;-12.49104;0.07501;,
 1.97427;-12.33151;-0.44598;,
 1.66024;-11.00806;1.14266;,
 0.53422;-11.55259;-0.42671;,
 1.86431;-12.50982;0.38607;,
 1.50301;-11.12757;-2.45144;,
 0.53422;-11.55259;-0.42671;,
 0.28069;-10.15331;-2.41045;,
 1.50301;-11.12757;-2.45144;,
 0.84869;-12.01052;-0.46271;,
 0.53422;-11.55259;-0.42671;,
 3.55958;-10.07961;-1.26825;,
 2.68341;-12.47692;-0.24219;,
 3.43714;-10.66305;-1.94332;,
 2.68341;-12.47692;-0.24219;,
 2.10980;-12.29277;-1.45276;,
 3.43714;-10.66305;-1.94332;,
 0.28069;-10.15331;-2.41045;,
 1.16673;-9.21349;-3.58722;,
 1.97381;-9.47915;-3.51521;,
 0.28069;-10.15331;-2.41045;,
 -1.03834;-7.05504;-4.51551;,
 1.16673;-9.21349;-3.58722;,
 0.90119;-10.26900;1.51142;,
 1.73123;-5.46399;4.70198;,
 -1.02974;-6.31702;3.85825;,
 -1.00171;-7.80018;0.78893;,
 0.90119;-10.26900;1.51142;,
 -1.02974;-6.31702;3.85825;,
 -1.03834;-7.05504;-4.51551;,
 0.28069;-10.15331;-2.41045;,
 -0.24469;-10.69765;-0.59040;,
 2.68341;-12.47692;-0.24219;,
 1.97427;-12.33151;-0.44598;,
 2.10980;-12.29277;-1.45276;,
 2.28072;-12.49104;0.07501;,
 0.84869;-12.01052;-0.46271;,
 1.16753;-12.46164;-0.44193;,
 1.16753;-12.46164;-0.44193;,
 0.84869;-12.01052;-0.46271;,
 1.50301;-11.12757;-2.45144;,
 2.28072;-12.49104;0.07501;,
 1.86431;-12.50982;0.38607;,
 0.84869;-12.01052;-0.46271;,
 3.20361;-10.78615;0.19549;,
 3.55958;-10.07961;-1.26825;,
 3.94849;-9.01234;-0.69217;,
 0.35667;-10.40256;0.33275;,
 -1.05323;-7.83762;-0.68747;,
 -0.24469;-10.69765;-0.59040;,
 -1.32259;-5.29111;3.25159;,
 -1.75087;-5.38253;-0.78557;,
 1.22769;-4.24698;4.16332;,
 3.49753;-3.85433;1.57275;,
 3.10212;-3.51165;-3.17147;,
 3.72181;-3.23021;-1.33474;,
 -1.02551;-5.59590;-4.65702;,
 1.63474;-4.31103;-4.27889;,
 0.75015;-0.35301;-1.16030;,
 0.06353;-0.03800;0.01161;,
 1.16229;-0.31770;-0.68769;,
 -0.77542;-0.66055;-1.18104;,
 0.11218;-0.51505;-1.33449;,
 -0.84593;-0.88120;0.88447;,
 -1.04483;-0.76442;-0.31217;,
 0.09038;-0.75426;1.42468;,
 1.05762;-0.46689;0.46981;,
 -1.28951;-3.03697;-3.88737;,
 0.88302;-2.51379;-3.60402;,
 -1.57538;-3.03523;2.49579;,
 -1.76901;-2.95019;-0.65715;,
 0.69271;-2.60833;3.26021;,
 2.42955;-2.00040;1.04282;,
 2.47818;-2.13902;-2.85956;,
 2.89205;-1.92902;-1.30101;;
 
 156;
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
 3;43,80,74;,
 3;74,80,157;,
 3;80,85,157;,
 3;85,89,157;,
 3;55,7,77;,
 3;77,7,19;,
 3;19,7,116;,
 3;116,7,1;,
 3;7,112,1;,
 3;165,166,167;,
 3;168,169,170;,
 3;171,172,173;,
 3;174,175,176;,
 3;177,178,179;,
 3;180,181,182;,
 3;183,184,185;,
 3;186,187,188;,
 3;189,190,191;,
 3;192,193,194;,
 3;195,196,197;,
 3;198,199,200;,
 3;201,202,203;,
 3;204,205,206;,
 3;207,208,209;,
 3;210,211,212;,
 3;213,214,215;,
 3;216,217,218;,
 3;219,220,221;,
 3;222,223,224;,
 3;225,226,227;,
 3;228,229,230;,
 3;231,232,233;,
 3;234,235,236;,
 3;237,238,239;,
 3;240,241,242;,
 3;243,244,245;,
 3;246,247,248;,
 3;249,250,251;,
 3;252,253,254;,
 3;255,256,257;,
 3;258,259,260;,
 3;261,262,263;,
 3;264,265,266;,
 3;267,268,269;,
 3;270,271,272;,
 3;273,274,275;,
 3;276,277,278;,
 3;279,280,281;,
 3;282,283,284;,
 3;285,286,287;,
 3;288,289,290;,
 3;291,292,293;,
 3;294,295,296;,
 3;297,298,299;,
 3;300,301,302;,
 3;303,304,305;,
 3;306,307,308;,
 3;309,310,311;,
 3;312,313,314;,
 3;315,316,317;,
 3;318,319,320;,
 3;321,322,323;,
 3;324,325,326;,
 3;305,327,248;,
 3;327,328,248;,
 3;328,261,248;,
 4;327,305,301,329;,
 4;272,330,329,301;,
 4;189,331,332,165;,
 4;194,333,334,245;,
 3;335,336,337;,
 3;338,336,339;,
 3;340,336,341;,
 3;336,340,342;,
 3;343,336,342;,
 4;333,344,345,334;,
 4;327,346,347,328;,
 4;346,327,329,348;,
 4;330,349,348,329;,
 4;331,350,351,332;,
 4;344,338,339,345;,
 4;346,340,341,347;,
 4;340,346,348,342;,
 4;349,343,342,348;,
 4;350,335,337,351;,
 3;245,334,182;,
 3;334,331,182;,
 3;331,189,182;,
 4;345,350,331,334;,
 4;339,335,350,345;,
 3;336,335,339;,
 4;328,333,194,261;,
 4;347,344,333,328;,
 4;341,338,344,347;,
 3;336,338,341;,
 4;332,330,272,165;,
 4;351,349,330,332;,
 4;337,343,349,351;,
 3;336,343,337;;
 
 MeshMaterialList {
  2;
  156;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  145;
  -0.924027;-0.169368;0.342765;,
  0.856603;0.206361;-0.472913;,
  0.852629;0.154691;-0.499094;,
  -0.649876;0.125593;-0.749591;,
  -0.754139;-0.207451;0.623087;,
  -0.572636;0.231759;-0.786369;,
  -0.897780;-0.222621;0.380040;,
  0.844021;0.183666;-0.503880;,
  0.892352;0.211268;-0.398841;,
  0.415821;-0.287525;0.862799;,
  -0.649632;0.117800;-0.751067;,
  -0.660269;0.138935;-0.738066;,
  -0.574850;0.233040;-0.784372;,
  0.835430;0.208240;-0.508619;,
  -0.890110;-0.218160;0.400139;,
  -0.686946;0.146727;-0.711742;,
  0.496780;-0.277354;0.822365;,
  0.884494;0.189589;-0.426293;,
  -0.636650;0.157547;-0.754888;,
  -0.903744;-0.231042;0.360371;,
  0.488647;-0.308145;0.816254;,
  -0.805487;-0.201395;0.557343;,
  0.875675;0.214352;-0.432720;,
  -0.596541;0.170303;-0.784306;,
  -0.868228;-0.182258;0.461479;,
  -0.586076;0.238433;-0.774380;,
  0.507401;-0.234031;0.829322;,
  0.810027;0.181537;-0.557585;,
  0.523510;-0.255428;0.812831;,
  0.881025;0.214366;-0.421714;,
  0.883236;0.128904;-0.450863;,
  -0.636852;0.161849;-0.753806;,
  -0.874986;-0.200615;0.440627;,
  0.516725;-0.285423;0.807174;,
  -0.819861;-0.184373;0.542066;,
  0.996587;0.082257;-0.007015;,
  0.845653;-0.290661;0.447647;,
  0.907289;0.131657;0.399366;,
  -0.665772;-0.460256;0.587292;,
  -0.835747;-0.461154;0.298099;,
  -0.630325;-0.549983;0.547913;,
  0.965302;-0.243704;0.093805;,
  -0.484892;-0.640462;-0.595557;,
  0.241087;-0.424517;0.872732;,
  0.533806;-0.253679;0.806659;,
  0.787190;0.102929;-0.608060;,
  0.518414;0.134381;-0.844505;,
  0.617082;-0.238882;-0.749764;,
  -0.271394;-0.646437;-0.713067;,
  0.495446;-0.515562;-0.699092;,
  0.201145;-0.493451;-0.846195;,
  -0.003573;-0.985547;-0.169366;,
  0.116089;-0.896876;-0.426775;,
  -0.244068;-0.886003;-0.394246;,
  0.915317;-0.270500;-0.298370;,
  0.264209;-0.124640;-0.956378;,
  -0.265075;-0.419170;-0.868350;,
  -0.106522;-0.506284;-0.855762;,
  0.938668;-0.335998;0.077511;,
  0.805415;-0.379368;0.455397;,
  0.877646;-0.458884;0.138431;,
  0.353309;-0.349213;0.867884;,
  0.337868;-0.118851;0.933659;,
  -0.730042;-0.466851;0.499089;,
  -0.713961;-0.699054;0.039771;,
  -0.679846;-0.459070;0.571895;,
  -0.935987;-0.314062;0.159040;,
  -0.918016;-0.380713;0.110918;,
  -0.748902;-0.478242;-0.458727;,
  -0.703544;-0.309953;0.639496;,
  -0.989542;-0.143813;-0.011147;,
  -0.981595;-0.164670;0.096726;,
  -0.395898;0.299942;0.867928;,
  0.784498;0.471519;0.402782;,
  0.896494;0.436831;-0.074005;,
  0.621842;0.385861;-0.681487;,
  0.274275;0.265171;-0.924369;,
  0.020660;0.188045;-0.981943;,
  0.152457;0.899424;-0.409625;,
  0.450672;0.890163;-0.067115;,
  -0.164094;0.985710;0.038058;,
  -0.503579;0.792648;-0.343681;,
  -0.115774;0.855181;-0.505234;,
  -0.613737;0.692560;0.379061;,
  -0.787125;0.616699;0.010801;,
  -0.036986;0.811977;0.582516;,
  0.435056;0.828896;0.351649;,
  0.068670;0.581591;-0.810578;,
  -0.074889;0.556230;-0.827647;,
  -0.986815;0.157275;-0.038209;,
  -0.880042;0.337078;0.334522;,
  -0.128938;0.585932;0.800036;,
  0.670637;0.625239;0.399152;,
  0.756566;0.647277;-0.092953;,
  0.474865;0.649155;-0.594224;,
  0.833464;0.174333;-0.524354;,
  0.844333;0.233767;-0.482136;,
  -0.732253;-0.207683;0.648594;,
  -0.825027;-0.228548;0.516813;,
  -0.596813;0.172821;-0.783548;,
  0.886829;0.118012;-0.446775;,
  -0.914099;-0.207310;0.348490;,
  0.541747;-0.205635;0.815000;,
  0.535054;-0.200947;0.820571;,
  0.542374;-0.223977;0.809732;,
  -0.886647;-0.169468;0.430276;,
  -0.742176;0.138453;-0.655748;,
  0.557304;-0.276435;0.782941;,
  0.549041;-0.287640;0.784740;,
  -0.918183;-0.152295;0.365714;,
  -0.597393;0.240018;-0.765188;,
  0.893883;0.113695;-0.433643;,
  0.539715;-0.288458;0.790886;,
  -0.018541;0.999538;0.024084;,
  0.087169;0.994598;0.056357;,
  -0.031025;0.996806;0.073582;,
  0.018475;0.995608;0.091781;,
  -0.076163;0.992851;0.091900;,
  0.150437;0.988122;0.031378;,
  0.295478;0.955348;0.001880;,
  0.503972;0.858883;-0.091284;,
  0.093869;-0.995230;-0.026575;,
  0.044528;-0.998997;0.004655;,
  0.070273;-0.996624;-0.042462;,
  0.114976;-0.992786;-0.034002;,
  0.136461;-0.990570;-0.012254;,
  -0.052101;-0.996869;0.059481;,
  -0.192760;-0.972272;0.132405;,
  -0.660983;-0.618586;-0.424797;,
  -0.644830;-0.660966;-0.383819;,
  0.942458;-0.317821;0.103744;,
  0.497710;-0.287385;0.818349;,
  -0.777077;-0.565597;0.276136;,
  0.579112;-0.302673;0.756980;,
  -0.973588;-0.209114;-0.091637;,
  -0.061940;-0.950822;-0.303482;,
  -0.160238;-0.977152;-0.139635;,
  -0.413088;-0.251648;0.875232;,
  -0.193469;-0.929139;-0.315074;,
  -0.398558;0.307285;0.864134;,
  0.633287;0.481361;0.606003;,
  -0.393209;0.292577;0.871657;,
  0.597813;0.523418;0.607168;,
  -0.986042;-0.083610;-0.143981;,
  -0.968754;0.167928;-0.182529;;
  156;
  3;25,5,12;,
  3;26,28,16;,
  3;24,4,34;,
  3;16,9,26;,
  3;5,15,31;,
  3;27,95,7;,
  3;17,8,96;,
  3;97,98,21;,
  3;11,99,18;,
  3;2,7,100;,
  3;20,9,16;,
  3;19,14,101;,
  3;31,15,18;,
  3;100,30,2;,
  3;10,3,11;,
  3;5,99,12;,
  3;13,29,7;,
  3;13,1,29;,
  3;102,103,104;,
  3;21,98,32;,
  3;32,98,101;,
  3;32,0,105;,
  3;15,106,18;,
  3;16,28,107;,
  3;16,108,33;,
  3;29,1,17;,
  3;18,106,10;,
  3;10,23,18;,
  3;19,0,32;,
  3;101,6,19;,
  3;20,16,33;,
  3;21,32,105;,
  3;34,105,109;,
  3;96,7,29;,
  3;96,22,17;,
  3;11,23,10;,
  3;18,23,11;,
  3;109,24,34;,
  3;12,110,25;,
  3;34,4,97;,
  3;26,102,104;,
  3;107,108,16;,
  3;2,27,7;,
  3;7,95,13;,
  3;104,28,26;,
  3;29,22,96;,
  3;17,22,29;,
  3;100,111,30;,
  3;31,99,5;,
  3;18,99,31;,
  3;101,14,32;,
  3;32,14,19;,
  3;33,112,20;,
  3;21,34,97;,
  3;21,105,34;,
  3;113,114,115;,
  3;114,116,115;,
  3;116,117,115;,
  3;115,117,118;,
  3;117,119,118;,
  3;119,120,118;,
  3;121,122,123;,
  3;123,122,124;,
  3;124,122,125;,
  3;125,122,126;,
  3;122,127,126;,
  3;35,37,36;,
  3;38,40,39;,
  3;35,36,41;,
  3;42,128,129;,
  3;43,36,44;,
  3;45,47,46;,
  3;48,50,49;,
  3;51,53,52;,
  3;45,35,54;,
  3;55,57,56;,
  3;48,42,50;,
  3;55,47,50;,
  3;57,55,50;,
  3;49,50,54;,
  3;58,130,54;,
  3;53,48,52;,
  3;48,49,52;,
  3;59,61,60;,
  3;131,62,43;,
  3;40,63,39;,
  3;64,65,38;,
  3;59,41,36;,
  3;132,66,63;,
  3;59,131,61;,
  3;47,45,54;,
  3;50,47,54;,
  3;46,47,55;,
  3;66,132,67;,
  3;131,59,36;,
  3;61,131,133;,
  3;63,40,132;,
  3;58,59,60;,
  3;66,134,39;,
  3;41,58,54;,
  3;35,41,54;,
  3;44,36,37;,
  3;135,53,51;,
  3;136,135,51;,
  3;40,38,65;,
  3;48,128,42;,
  3;68,64,128;,
  3;58,60,130;,
  3;60,52,49;,
  3;42,57,50;,
  3;42,56,57;,
  3;43,44,69;,
  3;67,132,69;,
  3;56,42,39;,
  3;136,51,52;,
  3;61,64,137;,
  3;53,64,48;,
  3;135,138,64;,
  3;59,58,41;,
  3;63,66,39;,
  3;69,71,67;,
  3;71,70,67;,
  3;70,66,67;,
  4;72,69,139,72;,
  4;37,73,140,44;,
  4;45,75,74,35;,
  4;56,77,76,55;,
  3;78,80,79;,
  3;81,80,82;,
  3;83,80,84;,
  3;80,83,85;,
  3;86,80,85;,
  4;77,88,87,76;,
  4;71,90,89,70;,
  4;141,72,72,91;,
  4;73,92,142,140;,
  4;75,94,93,74;,
  4;88,81,82,87;,
  4;90,83,84,89;,
  4;83,90,91,85;,
  4;92,86,85,91;,
  4;94,78,79,93;,
  3;55,76,46;,
  3;76,75,46;,
  3;75,45,46;,
  4;87,94,75,76;,
  4;82,78,94,87;,
  3;80,78,82;,
  4;70,143,134,66;,
  4;89,144,143,70;,
  4;84,81,144,89;,
  3;80,81,84;,
  4;74,73,37,35;,
  4;93,92,73,74;,
  4;79,86,92,93;,
  3;80,86,79;;
 }
 MeshTextureCoords {
  352;
  0.210984;0.888035;,
  0.210984;0.888035;,
  0.210984;0.888035;,
  0.169004;0.710560;,
  0.169004;0.710560;,
  0.169004;0.710560;,
  0.157133;0.591272;,
  0.157133;0.591272;,
  0.157133;0.591272;,
  0.191108;0.786201;,
  0.191108;0.786201;,
  0.191108;0.786201;,
  0.036545;0.818742;,
  0.036545;0.818742;,
  0.036545;0.818742;,
  0.167346;0.810942;,
  0.167346;0.810942;,
  0.167346;0.810942;,
  0.173407;0.708488;,
  0.173407;0.708488;,
  0.173407;0.708488;,
  0.169494;0.609970;,
  0.169494;0.609970;,
  0.169494;0.609970;,
  0.147457;0.624882;,
  0.147457;0.624882;,
  0.147457;0.624882;,
  0.093882;0.581865;,
  0.093882;0.581865;,
  0.093882;0.581865;,
  0.144598;0.551060;,
  0.144598;0.551060;,
  0.144598;0.551060;,
  0.143336;0.651212;,
  0.143336;0.651212;,
  0.143336;0.651212;,
  0.145319;0.539625;,
  0.145319;0.539625;,
  0.145319;0.539625;,
  0.162849;0.697008;,
  0.162849;0.697008;,
  0.162849;0.697008;,
  0.205767;0.829082;,
  0.205767;0.829082;,
  0.205767;0.829082;,
  0.029508;0.625852;,
  0.029508;0.625852;,
  0.029508;0.625852;,
  0.147660;0.553476;,
  0.147660;0.553476;,
  0.147660;0.553476;,
  0.205203;0.782657;,
  0.205203;0.782657;,
  0.205203;0.782657;,
  0.197636;0.977837;,
  0.197636;0.977837;,
  0.197636;0.977837;,
  0.170932;0.605398;,
  0.170932;0.605398;,
  0.170932;0.605398;,
  0.191814;0.770239;,
  0.191814;0.770239;,
  0.191814;0.770239;,
  0.147439;0.977571;,
  0.147439;0.977571;,
  0.147439;0.977571;,
  0.164358;0.692896;,
  0.164358;0.692896;,
  0.164358;0.692896;,
  0.140116;0.978274;,
  0.140116;0.978274;,
  0.140116;0.978274;,
  0.112667;0.656033;,
  0.112667;0.656033;,
  0.112667;0.656033;,
  0.196056;0.798839;,
  0.196056;0.798839;,
  0.196056;0.798839;,
  0.145451;0.575646;,
  0.145451;0.575646;,
  0.145451;0.575646;,
  0.191578;0.612192;,
  0.191578;0.612192;,
  0.191578;0.612192;,
  0.165094;0.669562;,
  0.165094;0.669562;,
  0.165094;0.669562;,
  0.189294;0.761964;,
  0.189294;0.761964;,
  0.189294;0.761964;,
  0.113444;0.638508;,
  0.113444;0.638508;,
  0.113444;0.638508;,
  0.208462;0.762240;,
  0.208462;0.762240;,
  0.208462;0.762240;,
  0.078482;0.573605;,
  0.078482;0.573605;,
  0.078482;0.573605;,
  0.049784;0.786387;,
  0.049784;0.786387;,
  0.049784;0.786387;,
  0.143559;0.744559;,
  0.143559;0.744559;,
  0.143559;0.744559;,
  0.140213;0.560201;,
  0.140213;0.560201;,
  0.140213;0.560201;,
  0.201608;0.651199;,
  0.201608;0.651199;,
  0.201608;0.651199;,
  0.152415;0.583579;,
  0.152415;0.583579;,
  0.152415;0.583579;,
  0.191527;0.810762;,
  0.191527;0.810762;,
  0.191527;0.810762;,
  0.117816;0.692782;,
  0.117816;0.692782;,
  0.117816;0.692782;,
  0.155610;0.825066;,
  0.155610;0.825066;,
  0.155610;0.825066;,
  0.071349;0.701677;,
  0.071349;0.701677;,
  0.071349;0.701677;,
  0.064644;0.706785;,
  0.064644;0.706785;,
  0.064644;0.706785;,
  0.149977;0.782300;,
  0.149977;0.782300;,
  0.149977;0.782300;,
  0.140995;0.782288;,
  0.140995;0.782288;,
  0.140995;0.782288;,
  0.020498;0.684991;,
  0.020498;0.684991;,
  0.020498;0.684991;,
  0.030216;0.530478;,
  0.030216;0.530478;,
  0.030216;0.530478;,
  0.178992;0.770185;,
  0.178992;0.770185;,
  0.178992;0.770185;,
  0.105313;0.739996;,
  0.105313;0.739996;,
  0.105313;0.739996;,
  0.160244;0.632591;,
  0.160244;0.632591;,
  0.160244;0.632591;,
  0.204565;0.820521;,
  0.204565;0.820521;,
  0.204565;0.820521;,
  0.122827;0.599349;,
  0.122827;0.599349;,
  0.122827;0.599349;,
  0.175065;0.759206;,
  0.175065;0.759206;,
  0.175065;0.759206;,
  0.118267;0.699955;,
  0.118267;0.699955;,
  0.118267;0.699955;,
  0.037573;0.850857;,
  0.037573;0.850857;,
  0.037573;0.850857;,
  0.583506;0.555796;,
  0.583506;0.555796;,
  0.584184;0.557388;,
  0.847588;0.541840;,
  0.847588;0.541840;,
  0.847588;0.541840;,
  0.670936;0.334495;,
  0.671614;0.336086;,
  0.671614;0.336086;,
  0.703715;0.097023;,
  0.703715;0.097023;,
  0.703715;0.097023;,
  0.634520;0.781960;,
  0.634520;0.781960;,
  0.634520;0.781960;,
  0.876351;0.602113;,
  0.876351;0.602113;,
  0.876351;0.602113;,
  0.727711;0.192762;,
  0.727711;0.192762;,
  0.727711;0.192762;,
  0.885730;0.575067;,
  0.885730;0.575067;,
  0.885730;0.575067;,
  0.822439;0.762350;,
  0.821761;0.760758;,
  0.822439;0.762350;,
  0.662202;0.465697;,
  0.662202;0.465697;,
  0.662202;0.465697;,
  0.824823;0.112773;,
  0.824823;0.112773;,
  0.824823;0.112773;,
  0.805609;0.922985;,
  0.805609;0.922985;,
  0.805609;0.922985;,
  0.842776;0.631205;,
  0.842776;0.631205;,
  0.842776;0.631205;,
  0.777859;0.576916;,
  0.777859;0.576916;,
  0.777859;0.576916;,
  0.896394;0.843585;,
  0.896394;0.843585;,
  0.896394;0.843585;,
  0.718500;0.248360;,
  0.718500;0.248360;,
  0.718500;0.248360;,
  0.623387;0.034183;,
  0.623387;0.034183;,
  0.623387;0.034183;,
  0.797714;0.872403;,
  0.797714;0.872403;,
  0.797714;0.872403;,
  0.540508;0.287577;,
  0.540508;0.287577;,
  0.540508;0.287577;,
  0.894712;0.729028;,
  0.894712;0.729028;,
  0.894712;0.729028;,
  0.749572;0.351957;,
  0.749572;0.351957;,
  0.749572;0.351957;,
  0.813718;0.664076;,
  0.813718;0.664076;,
  0.813718;0.664076;,
  0.727618;0.532344;,
  0.727618;0.532344;,
  0.727618;0.532344;,
  0.852974;0.692823;,
  0.852974;0.692823;,
  0.852974;0.692823;,
  0.812025;0.758314;,
  0.812025;0.758314;,
  0.812025;0.758314;,
  0.775054;0.633782;,
  0.775054;0.633782;,
  0.775054;0.633782;,
  0.664295;0.086999;,
  0.664295;0.086999;,
  0.664295;0.086999;,
  0.642404;0.623919;,
  0.642404;0.623919;,
  0.643276;0.622116;,
  0.776719;0.310676;,
  0.776719;0.310676;,
  0.776719;0.310676;,
  0.679393;0.130806;,
  0.679393;0.130806;,
  0.679393;0.130806;,
  0.746525;0.101050;,
  0.746525;0.101050;,
  0.746525;0.101050;,
  0.619653;0.120839;,
  0.619653;0.120839;,
  0.619653;0.120839;,
  0.769506;0.361181;,
  0.769506;0.361181;,
  0.769506;0.361181;,
  0.882473;0.849961;,
  0.882473;0.849961;,
  0.882473;0.849961;,
  0.762717;0.721781;,
  0.763395;0.723372;,
  0.763395;0.723372;,
  0.683498;0.040999;,
  0.684176;0.042591;,
  0.683498;0.040999;,
  0.856471;0.429890;,
  0.856471;0.429890;,
  0.856471;0.429890;,
  0.876936;0.285471;,
  0.876936;0.285471;,
  0.876936;0.285471;,
  0.894482;0.976149;,
  0.894482;0.976149;,
  0.894482;0.976149;,
  0.711628;0.514293;,
  0.711628;0.514293;,
  0.711628;0.514293;,
  0.867303;0.635151;,
  0.867303;0.635151;,
  0.867303;0.635151;,
  0.845801;0.756379;,
  0.845801;0.756379;,
  0.845801;0.756379;,
  0.652702;0.274367;,
  0.652702;0.274367;,
  0.652702;0.274367;,
  0.871327;0.465675;,
  0.871327;0.465675;,
  0.871327;0.465675;,
  0.591178;0.089186;,
  0.591178;0.089186;,
  0.591178;0.089186;,
  0.584382;0.117922;,
  0.584382;0.117922;,
  0.584382;0.117922;,
  0.693868;0.171667;,
  0.693868;0.171667;,
  0.693868;0.171667;,
  0.720270;0.024120;,
  0.720270;0.024120;,
  0.720270;0.024120;,
  0.857181;0.516613;,
  0.857181;0.516613;,
  0.857181;0.516613;,
  0.905645;0.493189;,
  0.905645;0.493189;,
  0.905645;0.493189;,
  0.789594;0.323703;,
  0.789594;0.323703;,
  0.789594;0.323703;,
  0.723331;0.092155;,
  0.723331;0.092155;,
  0.723331;0.092155;,
  0.824821;0.331857;,
  0.824821;0.331857;,
  0.824821;0.331857;,
  0.658349;0.097647;,
  0.658349;0.097647;,
  0.658349;0.097647;,
  0.681204;0.196895;,
  0.668821;0.313523;,
  0.594809;0.156572;,
  0.672083;0.097340;,
  0.792730;0.689368;,
  0.729360;0.378405;,
  0.656250;0.431909;,
  0.739216;0.588418;,
  0.667710;0.382252;,
  0.632088;0.294743;,
  0.659477;0.314621;,
  0.636034;0.317146;,
  0.654956;0.356037;,
  0.647329;0.264380;,
  0.641723;0.290568;,
  0.614309;0.228848;,
  0.647684;0.217763;,
  0.648745;0.389302;,
  0.705728;0.495866;,
  0.668694;0.221818;,
  0.658899;0.304047;,
  0.601691;0.182078;,
  0.656178;0.175842;,
  0.740212;0.560356;,
  0.697775;0.366176;;
 }
}
