#include "DatToVect.h"

DatToVect::DatToVect():Tool(){}


bool DatToVect::Initialise(std::string configfile, DataModel &data){
	
	if(configfile!="")  m_variables.Initialise(configfile);
	//m_variables.Print();
	
	m_data= &data;
	m_log= m_data->Log;
	
	if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
	
	m_variables.Get("input_file", input_file);    // location of input file.
	m_variables.Get("output_file", output_file);  // location of output files.
	
	if(!m_variables.Get("muons_per_file", muons_per_file)) muons_per_file = -1;  // Number of events for each output file
	
	std::cout << "Opening input file: " << input_file << std::endl;
	// Open the input file
	i_file.open(input_file);
	
	// Check if the files were successfully opened
	if (!i_file.is_open()) {
		std::cerr << "Error: could not open " << input_file << std::endl;
		return false;
	}
	
	// Skip the first line of the input file
	std::getline(i_file, line);
	
	if(muons_per_file == -1){
		//Add .dat to output filename
		std::string output_name = output_file + ".dat";
		// Print opening file and filename
		std::cout << "Set to single output mode - Opening " << output_name << std::endl;
		// Open a single output file to put all of the primaries in
		o_file.open(output_name);
		if(!o_file.is_open()){
			std::cerr << "Error: could not open " << output_name << std::endl;
		}
	}
	
	return true;
}


bool DatToVect::Execute(){
	if(muons_per_file == -1){
		// Call GeneratePrimary with the single output file
		GeneratePrimary(o_file);
	}else{
		// If muons_in_file is -1 then a new file needs opening 
		if(muons_in_file == -1){
			// Create a new output name with the new file number
			std::string new_output_name = output_file + std::to_string(file_number) + ".dat";
			// Open the file
			new_o_file.open(new_output_name);
			// Increment the file number
			file_number++;
			// Set the muons_in_file counter to 0
			muons_in_file = 0;
		}
		// Call generate primary to populate the file with a primary
		GeneratePrimary(new_o_file);
		// Incrememnt the number of muons in the current file
		muons_in_file++;
		// Check if the number of muons in the current file is equal to the number of muons wanted in each file
		if(muons_in_file == muons_per_file){
			new_o_file.close();
			muons_in_file = -1;
		}
	}
	
	return true;
}


bool DatToVect::Finalise(){
	i_file.close();
	if(muons_per_file == -1){
		o_file.close();
	}
	return true;
}

bool DatToVect::GeneratePrimary(std::ofstream& target_file){
	// Read the file line by line
	if(!std::getline(i_file, line)){
		m_data->vars.Set("StopLoop", true);
		std::cout << "Reached the end of the file" << std::endl;
		return true;
	}
	
	// Split the line by spaces
	std::stringstream row(line);
	
	// Initialise the variables from the muon FLUKA output files
	int mu_count, mu_number;
	float ent_ene, exit_ene;
	float ent_x, ent_y, ent_z;
	float exit_x, exit_y, exit_z;
	
	// Store the data from the FLUKA output in the variables
	row >> mu_count\
		>> mu_number\
		>> ent_ene\
		>> exit_ene\
		>> ent_x\
		>> ent_y\
		>> ent_z\
		>> exit_x\
		>> exit_y\
		>> exit_z;
	
	ent_ene = ent_ene * 1000;
	
	std::cout << "Mu count: " << mu_count << std::endl;
	ent_x = ent_x * 10;
	ent_y = ent_y * 10;
	ent_z = ent_z * 10;
	exit_x = exit_x * 10;
	exit_y = exit_y * 10;
	exit_z = exit_z * 10;
	
	float dir_x = exit_x - ent_x;
	float dir_y = exit_y - ent_y;
	float dir_z = exit_z - ent_z;
	
	float mag_dir = sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
	
	
	dir_x = (dir_x)/(mag_dir);
	dir_y = (dir_y)/(mag_dir);
	dir_z = (dir_z)/(mag_dir);
	
	
	int t = 0;
	
	target_file << 0 << " "\
			<< mu_pdg << " "\
			<< ent_ene << " "\
			<< ent_x << " "\
			<< ent_y << " "\
			<< ent_z << " "\
			<< dir_x << " "\
			<< dir_y << " "\
			<< dir_z << " "\
			<< t << " \n";
	
	return true;
}