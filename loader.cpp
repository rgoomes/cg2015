#include "loader.hpp"

Loader::Loader(){

}

GLuint Loader::get_texture(string path){
	if(textures.find(path) == textures.end()){
		if(path.find(".bmp") != string::npos){
			return textures[path] = loadBMP(path.c_str());
			printf("LOAD BMP\n");
		}
		else
			return textures[path] = loadDDS(path.c_str());
	}else{
		return textures[path];
	}

}

GLuint Loader::get_shaders(string pathVertex, string pathFragment){
	if(shaders.find(pair<string, string>(pathVertex, pathFragment)) == shaders.end()){
		return shaders[pair<string, string>(pathVertex, pathFragment)] = load_shaders(pathVertex.c_str(), pathFragment.c_str());
	}else{
		return shaders[pair<string, string>(pathVertex, pathFragment)];
	}
}

Model* Loader::get_model(string path, float scale){
	this->s = scale;
	if(models.find(path) == models.end()){
		models[path] = load_model(path);
		return models[path];
	}else
		return models[path];
}

Model* Loader::load_model(string path){
	normals.clear();
	uvs.clear();
	out_vertices.clear();
	out_normals.clear();
	out_tangents.clear();
	out_bitangents.clear();
	out_uvs.clear();
	faces.clear();
	all_faces.clear();
	vertices.clear();
	groups.clear();
	texture_path = "";

	this->path = path;
	int l = this->path.find_last_of("/");
	this->name = this->path.substr(l+1, string::npos);
	this->texture_path = this->path + "/" + this->name + ".dds";
	this->obj_path = this->path + "/" + this->name + ".obj";
	
	Model *model = new Model;
	FILE *file = fopen(obj_path.c_str(), "r");

	if(!file)
		return model;

	load_mtl(this->path, this->name);

	int tmp=1;
	char buffer[1000];
	char line[256], *tmp_c;
	string texture_path;
	while(fscanf(file, "%s", line) != EOF){
		Point coord;
		Point2 coord2;
		Face face;

		if(!strcmp(line, "v")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			coord.x *= s; coord.y *= s; coord.z *= s;
			vertices.push_back(coord);
		} else if(!strcmp(line, "vn")){
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			this->normals.push_back(coord);
		} else if(!strcmp(line, "vt")){
			tmp = fscanf(file, "%f %f", &coord2.x, &coord2.y);
			this->uvs.push_back(coord2);
		} else if(!strcmp(line, "f")){
			tmp = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &face.v_index[0], &face.t_index[0], &face.n_index[0],
			&face.v_index[1], &face.t_index[1], &face.n_index[1], &face.v_index[2], &face.t_index[2], &face.n_index[2]);

			this->faces.push_back(face);
		}else if(!strcmp(line, "#")){
			tmp_c = fgets(buffer, 1000, file);
			if(tmp_c == NULL) return model;
		}else if(!strcmp(line, "g")){
			char group_name[30];
			tmp = fscanf(file, "%s", group_name);
			
			Group g = load_group(group_name);

			groups.push_back(g);
		}else if(!strcmp(line, "usemtl")){
			char mat[30];
			fscanf(file, "%s", mat);
			cur_material = mat;
		}

		if(tmp <= 0)
			return model;
	}

	Group g = load_group("");
	if(g.size > 0)
		groups.push_back(g);
	printf("Loaded %s\n", obj_path.c_str());

	fclose(file);

	model->vertices = vertices;
	model->all_faces = all_faces;
	model->groups = groups;

	cur_material = "";
	return model;
}

void Loader::set_material_ids(Group &g, Material& m){
	g.Ns_id = glGetUniformLocation(g.program_id, "Ns");
	g.Tf_id = glGetUniformLocation(g.program_id, "Tf");
	g.texture_id = glGetUniformLocation(g.program_id, "mytextureSampler");
	g.bump_id = glGetUniformLocation(g.program_id, "bumpSampler");
}

Group Loader::load_group(string group_name){
	out_vertices.clear();
	out_normals.clear();
	out_uvs.clear();
	out_bitangents.clear();
	out_tangents.clear();
	for(uint64_t i = 0; i < this->faces.size(); i++){
		out_vertices.push_back(vertices	[faces[i].v_index[0] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[0] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[0] - 1]);
		out_vertices.push_back(vertices	[faces[i].v_index[1] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[1] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[1] - 1]);
		out_vertices.push_back(vertices	[faces[i].v_index[2] - 1]);
		out_normals.push_back(normals 	[faces[i].n_index[2] - 1]);
		out_uvs.push_back(uvs 			[faces[i].t_index[2] - 1]);
		calcTangentSpace(out_tangents, out_bitangents, out_vertices, out_uvs);
	}
	
	Group g;
	g.program_id = get_shaders( "objects/textureVertexShader.glsl", "objects/textureFragmentShader.glsl" );
	g.shadow_program_id = get_shaders( "objects/depthVertexShader.glsl", "objects/depthFragmentShader.glsl" );
	g.matrix_id = glGetUniformLocation(g.program_id, "MVP");
	g.depthbias_id = glGetUniformLocation(g.program_id, "DepthBiasMVP");
	g.shadowmap_id = glGetUniformLocation(g.program_id, "shadowMap");
	g.viewmatrix_id = glGetUniformLocation(g.program_id, "V");
	g.modelmatrix_id = glGetUniformLocation(g.program_id, "M");
	g.lightdir_id = glGetUniformLocation(g.program_id, "LightInvDirection_worldspace");
	g.has_texture_id = glGetUniformLocation(g.program_id, "has_texture");
	g.has_bump_id = glGetUniformLocation(g.program_id, "has_bump");
	g.bumpfix_id = glGetUniformLocation(g.program_id, "bumpfix");
	g.shadows_id = glGetUniformLocation(g.program_id, "shadows");

	g.vertexposition_modelspace_id = glGetAttribLocation(g.program_id, "vertexPosition_modelspace");
	g.vertexUV_id = glGetAttribLocation(g.program_id, "vertexUV");
	g.normal_id = glGetAttribLocation(g.program_id, "vertexNormal_modelspace");
	g.tangent_id = glGetAttribLocation(g.program_id, "vertexTangent_modelspace");
	g.bitangent_id = glGetAttribLocation(g.program_id, "vertexBitangent_modelspace");

	g.size = out_vertices.size();
	texture_path = path + "/" + group_name + ".dds";
	if(cur_material != ""){
		g.material = materials[cur_material];
	}
	else
		init_material(g.material);
	set_material_ids(g, g.material);


	g.shadow_matrix_id = glGetUniformLocation(g.shadow_program_id, "depthMVP");

	
	glGenBuffers(1, &g.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size()*sizeof(Point), out_vertices.data(), GL_DYNAMIC_DRAW);
	
	glGenBuffers(1, &g.uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size()*sizeof(Point2), out_uvs.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &g.normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size()*sizeof(Point), out_normals.data(), GL_DYNAMIC_DRAW);

	if(g.material.bump != 0){
		glGenBuffers(1, &g.tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, g.tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, out_tangents.size()*sizeof(Point), out_tangents.data(), GL_DYNAMIC_DRAW);

		glGenBuffers(1, &g.bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, g.bitangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, out_bitangents.size()*sizeof(Point), out_bitangents.data(), GL_DYNAMIC_DRAW);		
	}
	
	all_faces.insert(all_faces.end(),faces.begin(),faces.end());

	faces.clear();
	return g;
}

void Loader::init_material(Material& m){
	m.illum = 1;
	m.Ka = vec3(0.4, 0.4, 0.4);	// ambient color
	m.Kd = vec3(0.6, 0.6, 0.6);	// diffuse color
	m.Tf = 1.0f; 				// transmission (average of 3 components)
	m.Ks = vec3(0.9, 0.9, 0.9);	// specular color
	m.Ns = 16.0f; 				// specular exponent
	m.map_Kd = 0; 				// texture
	m.bump = 0; 				// bump texture
	//m.Ni = 1.0f; 				// 
}

void Loader::load_mtl(string path, string name){
	string mtl_path = path + "/" + name + ".mtl";
	FILE *file = fopen(mtl_path.c_str(), "r");	

	if(!file)
		return;

	Material m;
	init_material(m);
	string mname = "";
	char p[64];
	while(1){
		int read = fscanf(file, "%s", p);
		if(read <= 0)
			break;
		string prop = p;
		if(prop == "newmtl"){
			if(mname != ""){
				materials[mname] = m;
				init_material(m);
			}
			fscanf(file, "%s", p);
			mname = p;
		}else if(prop == "illum"){
			fscanf(file, "%d", &m.illum);
		}else if(prop == "Ka"){
			fscanf(file, "%f %f %f", &m.Ka.x, &m.Ka.y, &m.Ka.z);
		}else if(prop == "Kd"){
			fscanf(file, "%f %f %f", &m.Kd.x, &m.Kd.y, &m.Kd.z);
		}else if(prop == "Ks"){
			fscanf(file, "%f %f %f", &m.Ks.x, &m.Ks.y, &m.Ks.z);
		}else if(prop == "Tf"){
			fscanf(file, "%f %f %f", &m.Tf, &m.Tf, &m.Tf);
		}else if(prop == "Ns"){
			fscanf(file, "%f", &m.Ns);
		}else if(prop == "map_Kd"){
			fscanf(file, "%s", p);
			m.map_Kd = get_texture(path + "/" + p);
		}else if(prop == "bump"){
			fscanf(file, "%s", p);
			printf("%s\n", p);
			m.bump = get_texture(path + "/" + p);
		}else{
			fgets(p, 64, file);
		}
	}

	materials[mname] = m;
}


void Loader::calcTangentSpace(
	// outputs
	std::vector<Point> & tangents,
	std::vector<Point> & bitangents,
	// inputs
	std::vector<Point> & vertices,
	std::vector<Point2> & uvs
){
	int i = vertices.size() - 1;

	// vertices
	glm::vec3 v0 = glm::vec3(vertices[i-2].x, vertices[i-2].y, vertices[i-2].z);
	glm::vec3 v1 = glm::vec3(vertices[i-1].x, vertices[i-1].y, vertices[i-1].z);
	glm::vec3 v2 = glm::vec3(vertices[ i ].x, vertices[ i ].y, vertices[ i ].z);

	// UVs
	glm::vec2 uv0 = glm::vec2(uvs[i-2].x, uvs[i-2].y);
	glm::vec2 uv1 = glm::vec2(uvs[i-1].x, uvs[i-1].y);
	glm::vec2 uv2 = glm::vec2(uvs[ i ].x, uvs[ i ].y);
	//printf("%f\n", uv2.y);

	// Edges of the triangle : position delta
	glm::vec3 deltaPos1 = v1-v0;
	glm::vec3 deltaPos2 = v2-v0;
	
	// UV delta
	glm::vec2 deltaUV1 = uv1-uv0;
	glm::vec2 deltaUV2 = uv2-uv0;


	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tg = (deltaPos1    - deltaPos2 )*r;
	glm::vec3 btg = (deltaPos2   - deltaPos1 )*r;

	Point tangent = {tg.x, tg.y, tg.z};
	Point bitangent = {btg.x, btg.y, btg.z};

	tangents.push_back(tangent);
	tangents.push_back(tangent);
	tangents.push_back(tangent);
	//printf("%f %f %f\n", tangent.x, tangent.y, tangent.z);

	bitangents.push_back(bitangent);
	bitangents.push_back(bitangent);
	bitangents.push_back(bitangent);

	// See "Going Further"
	/*for (unsigned int i=0; i<vertices.size(); i+=1 )
	{
		glm::vec3 & n = normals[i];
		glm::vec3 & t = tangents[i];
		glm::vec3 & b = bitangents[i];
		
		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));
		
		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f){
			t = t * -1.0f;
		}

	}*/
}