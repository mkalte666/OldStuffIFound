/*
File: setting.h
Purpose: Contains the template-class for Setting-loading. 
Author(s): Malte Kieﬂling (mkalte666)
*/

#pragma once
#include "base.h"

template<class T>
class setting 
{
public:
	setting(const char* ident);
	setting(const char* ident, const char* file);
	
	void SaveSetting(void);
	void SaveSetting(const char* destfile);
	void SaveSetting(T *value);
	void SaveSetting(T *value, const char* destfile);

	T GetSetting(void);
	T GetSetting(const char* filename);
	T GetSettingFast(void);
	
private:
	T m_data;
	const char* m_datafile;
	const char* m_identifier;
};

/*
* @name: setting
* @param: void
* @return: void
* @description: constructor for the setting-class. Loads Given Setting-type from standart-path and also sets the Standart-path
*/
template<class T>
setting<T>::setting(const char* ident)
{
	m_identifier = ident;
	m_datafile = "./cfg/settings/main.cfg";
	m_data = GetSetting();
}

/*
* @name: setting
* @param: const char* file: name of the file containing the setting
* @return: void
* @description: Constructor for the setting-class. Loads given Settin-type from given file and sets the path to it.
*/
template<class T>
setting<T>::setting(const char* ident, const char* file)
{
	m_identifier = ident;
	m_datafile = file;
	m_data = GetSetting();
}

/*
* @name: GetSetting
* @param: void
* @return: T
* @description: Returns T, reads it from the file on runtime
*/
template<class T>
T setting<T>::GetSetting(void)
{
	if(!m_datafile) //Ohhh, there is no file... Lets exit, only to be shure nothing BAD happens...
		return NULL;
	std::ifstream file(m_datafile, std::ios::in);
	T tmpdata;
	if(!file.is_open()) { //Oh, well, the given file doesn't exist. Lets Print a error and continue
		std::cout << "Faild opening file for Settings-read: '" << m_datafile << "' while reading '" << m_identifier << "' \n";
		return NULL;
	}

	char buf[BUFFER_SIZE];
	//File syntax is 'ident=data;', so our delims are '=' and ';', what makes 2 buffers. 
	while(!file.eof()) {
		std::string identbuffer;
		std::string databuffer;
		file.getline(buf, BUFFER_SIZE, '=');
		if(buf[0]=='\n' || buf[0]==' ' || buf[0] == '#') {
			file.ignore(1, '\n');
			continue;
		} else {
			identbuffer = buf;
			file.getline(buf, BUFFER_SIZE, ';');
			databuffer = buf;
			file.ignore(1,'\n');
			if(identbuffer.compare(m_identifier) == 0) {
				std::stringstream rawdata;
			
				rawdata << (databuffer); //We trust the Programmer to use stringstream compatible types (hopefully...)
				rawdata >> tmpdata;
			}
		}
	}
	file.close();
	m_data = tmpdata;
	return m_data;
}

/*
* @name: GetSetting
* @param: const char* file: File to read from
* @return: T
* @description: Returns T, reads it from the file on runtime. DOES NOT CHANGE THE PATH OF THE STORED FILE (m_datafile)
*/
template<class T>
T setting<T>::GetSetting(const char* filename)
{
	if(!file) //Ohhh, there is no file... Lets exit, only to be shure nothing BAD happens...
		return NULL;
	std::ifstream file(filename, ios::in);
	T tmpdata;
	if(!file.is_open()) { //Oh, well, the given file doesn't exist. Lets Print a error and continue
		std::cout << "Faild opening file for Settings-read: '" << m_datafile << "' while reading '" << m_identifier << "' \n";
		return NULL;
	}
	char buf[BUFFER_SIZE];
	//File syntax is 'ident=data;', so our delims are '=' and ';', what makes 2 buffers. 
	while(!file.eof()) {
		std::string identbuffer;
		std::string databuffer;
		file.getline(buf, BUFFER_SIZE, '=');
		identbuffer = buf;
		file.getline(buf, BUFFER_SIZE, ';');
		databuffer = buf;
		file.ignore(1,'\n');
		if(identbuffer.compare(m_identifier) == 0) {
			std::stringstream rawdata(databuffer); //We trust the Programmer to use stringstream compatible types (hopefully...)
			rawdata >> tmpdata;
		}
	}
	file.close();
	m_data = tmpdata;
	return m_data;
}

/*
* @name: GetSettingFast
* @param: void
* @return: T
* @description: Returns T stored in the class. Much faster, BUT CAN'T READ CHANGES FROM FILE!
*/
template<class T>
T setting<T>::GetSettingFast(void)
{
	return m_data;
}

/*
* @name: SaveSetting
* @param: void
* @return: void
* @description: Writes Setting to the given file.
*/
template<class T>
void setting<T>::SaveSetting(void)
{
	std::ifstream infile(m_datafile, std::ios::in |std::ios::out);
	if(!infile.is_open()) {
		std::cout << "couldnt Open '" << m_datafile << "' to write Setting '" <<  m_identifier << "' with value '" << m_data << "' in it!\n";
	}
	std::string outdata;

	char buf[BUFFER_SIZE];
	bool pass = false;
	while(!infile.eof()) {
		if(pass)
			outdata+="\n";
		std::string identbuffer;
		std::string databuffer;
		infile.getline(buf, BUFFER_SIZE, '=');
		identbuffer=buf;
		outdata+=identbuffer+"=";
		infile.getline(buf, BUFFER_SIZE, ';');
		databuffer=buf;
		if(identbuffer.compare(m_identifier)==0) {
			std::stringstream outraw;
			outraw << m_data;
			outdata+=outraw.str();
		}
		else outdata+=databuffer;
		outdata += ";";
		pass = true;
		infile.ignore(1, '\n');
	}
	infile.close();
	std::ofstream outfile(m_datafile, std::ios::in |std::ios::out);
	outfile.clear();
	outfile << outdata;
	outfile.close();
}

/*
* @name: SaveSetting
* @param: const char* destfile: destination to write to
* @return: void
* @description: Writes Setting to the given file WITHOUT changing the saved filepath
*/
template<class T>
void setting<T>::SaveSetting(const char* destfile)
{
	std::ifstream infile(destfile, std::ios::in |std::ios::out);
	if(!infile.is_open()) {
		std::cout << "couldnt Open '" << destfile << "' to write Setting '" <<  m_identifier << "' with value '" << m_data << "' in it!\n";
	}
	std::string outdata;
	while(!infile.eof()) {
		char buf[BUFFER_SIZE];
		std::string identbuffer;
		std::string databuffer;
		infile.getline(buf, BUFFER_SIZE, '=');
		identbuffer=buf;
		outdata+=identbuffer+"=";
		infile.getline(buf, BUFFER_SIZE, ';');
		databuffer=buf;
		if(identbuffer.compare(m_identifier)==0) {
			std::stringstream outraw;
			outraw << m_data;
			outdata+=outraw.str();
		}
		else outdata+=databuffer;
		outdata += ";\n";
		infile.ignore(1, '\n');
	}
	infile.close();
	std::ofstream outfile(destfile, std::ios::in |std::ios::out);
	outfile.clear();
	outfile << outdata;
	outfile.close();
	
}

/*
* @name: SaveSetting
* @param: T *value: data to set m_data to.
* @return: void
* @description: Sets m_data and writes Setting to the given file.
*/
template<class T>
void setting<T>::SaveSetting(T *value)
{
	m_data = *value;
	SaveSetting();
}

/*
* @name: SaveSetting
* @param:	T *value: data to set m_data to.
			const char* destfile: path to write to
* @return: void
* @description: Sets m_data and writes Setting to the given file WITHOUT changing the saved filepath
*/
template<class T>
void setting<T>::SaveSetting(T *value, const char* destfile)
{
	m_data = *value;
	SaveSetting(destfile);
}