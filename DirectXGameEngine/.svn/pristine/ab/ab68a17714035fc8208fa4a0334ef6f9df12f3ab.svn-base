// consoleTestAp.cpp : Defines the entry point for the console application.
//

#define STRICT
#include <fstream>
#include <iostream>
#include <vector>

#include <stdio.h>

int main(int argc, char **argv)
{
	int test1(6);
    int test2 = 7 + 8 * 5;

	auto func = [test1, test2] { std::cout << "The no is: " << test1 << ", " << test2 << std::endl; };
	func();

	system("PAUSE");

	return 0;
}

/*
#define STRICT
#include <fstream>
#include <iostream>

#include <stdio.h>


unsigned int ConvertEndianness( unsigned long value )
{
	return (((value&0x000000FF)<<24)+((value&0x0000FF00)<<8)+
			((value&0x00FF0000)>>8)+((value&0xFF000000)>>24));
}


int main(int argc, char **argv)
{ 
	FILE *pFile = fopen("C:/screen shots/picture001.png", "rb");

	struct size
	{
		unsigned int width, height;
	};

	size tmp;

	fseek( pFile, 16, SEEK_SET );
	fread( &tmp, 1, sizeof(tmp), pFile );
	//std::ifstream in( "C:\\screen shots\\picture001.png" );

	//in.seekg(16);
	//in.read((char *)&width, 4);
	//in.read((char *)&height, 4);

	tmp.width = ConvertEndianness(tmp.width);
	tmp.height = ConvertEndianness(tmp.height);

	std::cout << "C:\\screen shots\\picture001.png" << " is " << tmp.width << " pixels wide and " << tmp.height << " pixels high.\n";

	system("PAUSE");

	return 0;
}
*/


/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>
#include <iostream>
#include <string>
#include <utilities/genfunc.h>
using namespace std;


int main(int argc, char **argv)
{

	WORD version = GetVersion();
	WORD major = (WORD) (LOBYTE(version));
	WORD minor = (WORD) (HIBYTE(version));

	cout << "Major: " << major << "; Minor: " << minor << endl;

	system("PAUSE");

	return 0;
}
*/


/*
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;



int main(int argc, char **argv)
{



	system("PAUSE");

	return 0;
}
*/

/*
int main(int argc, char **argv)
{
	int StudentAge;

	try {
		cout << "Student Age: ";
		cin >> StudentAge;

		if(StudentAge < 0)
			throw exception( "Positive Number Required!" );

		cout << "\nStudent Age: " << StudentAge << "\n\n";
	}
	catch( std::exception & ex )
	{
		cout << "Error: " << ex.=.what();
	}

	cout << "\n";



	system("PAUSE");

	return 0;
}
*/

/*
#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & degrees;
        ar & minutes;
        ar & seconds;
    }
    int degrees;
    int minutes;
    float seconds;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {}
};

int main() {
    // create and open a character archive for output
    std::ofstream ofs("c:/serialized.txt");

    // create class instance
    const gps_position g(35, 59, 24.567f);

    // save data to archive
    {
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs("c:/serialized.txt");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> newg;
        // archive and stream closed when destructors are called
    }
    return 0;
}
*/


/*
#include <fstream>

// include headers that implement a archive in simple text format
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include <boost/filesystem.hpp>

#include "boost/date_time/posix_time/posix_time.hpp"

#include <boost/scoped_array.hpp>

#include <utilities/genfunc.h>

#include <ShlObj.h>

//#include <boost/serialization/nvp.hpp>

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class CGPS_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar 
			& BOOST_SERIALIZATION_NVP(degrees)
            & BOOST_SERIALIZATION_NVP(minutes)
            & BOOST_SERIALIZATION_NVP(seconds)
		    & BOOST_SERIALIZATION_NVP(vecString)
			// or
			//& boost::serialization::make_nvp("the_degrees", degrees)
			//& boost::serialization::make_nvp("the_minutes", minutes)
			//& boost::serialization::make_nvp("the_seconds", seconds)
			//& boost::serialization::make_nvp("the_vecString", vecString)
			
			;
    }
    int degrees;
    int minutes;
    float seconds;
	std::vector<std::string> vecString;
public:
    CGPS_position(){};
    CGPS_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {   vecString.push_back("I am the walrus");
		vecString.push_back("candy");
		vecString.push_back("jaw breakers");
		vecString.push_back("lemon heads"); }
};

int main()
{

	//wchar_t* path = NULL;
	//SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

	//char charPath[MAX_PATH];
	//wcstombs( charPath, path, sizeof(charPath) );
	// mbstowcs - Convert multibyte string to wide-character string

	//CoTaskMemFree(path);

	std::string path = NGenFunc::GetWindowsFolderPath( FOLDERID_Documents );

	bool tmp = boost::filesystem::is_regular_file( path + "\\GameMaker\\controllerMapping.cfg" );

	std::time_t time = boost::filesystem::last_write_time(path + "\\GameMaker\\controllerMapping.cfg");

	char timeChar[200];
	std::strftime( timeChar, 200, "%H:%M%p  %m/%d/%y", std::localtime(&time) );

	std::string timeStr = timeChar;
	std::transform(timeStr.begin(), timeStr.end(), timeStr.begin(), ::tolower);

	//boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();

	boost::posix_time::time_duration duration(0,0,0);

	boost::posix_time::time_duration conversion( boost::posix_time::duration_from_string( boost::posix_time::to_simple_string(duration) ) );
	//boost::posix_time::time_duration conversion( boost::posix_time::duration_from_string("0") );

	
	std::cout << "Time Duration: " << conversion.hours() << ":" << conversion.minutes() << std::endl;
	std::cout << "As string: " << boost::posix_time::to_simple_string(duration) << std::endl;

	//boost::posix_time::time_duration now(0,14,32);

	//boost::posix_time::time_duration later(150,24,55);

	//boost::posix_time::time_duration remaining = later - now;

	//boost::posix_time::time_duration remaining2( boost::posix_time::duration_from_string(boost::posix_time::to_simple_string(remaining)) );

	//std::cout << "Now the modification time is " << timeStr << std::endl;
	//std::cout << "Time Difference: " << remaining2.hours() << ":" << remaining2.minutes() << std::endl;
	//std::cout << "As string: " << boost::posix_time::to_simple_string(remaining2) << std::endl;

	//boost::filesystem::path directory( path + "/RabidSquirrelGames/Lost" );
	boost::filesystem::path directory( "./" );
	boost::filesystem::directory_iterator iter(directory), end;

	for(;iter != end; ++iter)
		std::cout << "file NAme: " << iter->path().filename().string() << std::endl;


    // save data to archive
    {
		CGPS_position gps_position(35, 59, 24.567f);

		std::ofstream ofs("c:/serialized.xml");
        boost::archive::xml_oarchive oa(ofs);
        // write class instance to archive
        oa << BOOST_SERIALIZATION_NVP(gps_position);
		// or
		//oa << boost::serialization::make_nvp("the_gps_position", gps_position);
    	// archive and stream closed when destructors are called
    }

    // read data from archive
    {
		CGPS_position gps_position;

        // create and open an archive for input
        std::ifstream ifs("c:/serialized.xml");
        boost::archive::xml_iarchive ia(ifs);
        // read class state from archive
		ia >> BOOST_SERIALIZATION_NVP(gps_position);
		// or
		//ia >> boost::serialization::make_nvp("the_gps_position", gps_position);
        // archive and stream closed when destructors are called
    }

	system("PAUSE");

    return 0;
}

*/


/*
 // Boost serialization Example
#include <fstream>

#include <boost/serialization/vector.hpp>

// a portable text archive
#include <boost/archive/text_oarchive.hpp> // saving
#include <boost/archive/text_iarchive.hpp> // loading

// a portable text archive using a wide character stream
#include <boost/archive/text_woarchive.hpp> // saving
#include <boost/archive/text_wiarchive.hpp> // loading

// a portable XML archive
#include <boost/archive/xml_oarchive.hpp> // saving
#include <boost/archive/xml_iarchive.hpp> // loading

// a portable XML archive which uses wide characters - use for utf-8 output
#include <boost/archive/xml_woarchive.hpp> // saving
#include <boost/archive/xml_wiarchive.hpp> // loading

// a non-portable native binary archive
#include <boost/archive/binary_oarchive.hpp> // saving
#include <boost/archive/binary_iarchive.hpp> // loading


/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar
			& degrees
			& minutes
            & seconds
			& testVec;
    }
    int degrees;
    int minutes;
    float seconds;
	std::vector<int> testVec;
public:
    gps_position(){};
    gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
    {   testVec.push_back(0);
		testVec.push_back(1);
		testVec.push_back(2);
		testVec.push_back(3);
		testVec.push_back(4);
		testVec.push_back(5);
		testVec.push_back(6);
		testVec.push_back(7);
		testVec.push_back(8);
		testVec.push_back(9); }
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(gps_position)

int main() {
    // create and open a character archive for output
    std::ofstream ofs_txt("c:\\serialized.txt");
	//std::ofstream ofs_bin("c:\\serialized.dat");
	//std::ofstream ofs_xml("c:\\serialized.xml");

    // create class instance
    const gps_position g(35, 59, 24.567f);

    // save data to archive
    {
        
		boost::archive::text_oarchive oa_txt(ofs_txt);
		//boost::archive::binary_oarchive oa_bin(ofs_bin);
		//boost::archive::text_oarchive oa_xml(ofs_xml);
        // write class instance to archive
        oa_txt << g;
		//oa_bin << g;
		//oa_xml << g;
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    gps_position newg;
    {
        // create and open an archive for input
        std::ifstream ifs_txt("c:\\serialized.txt");
		//std::ifstream ifs_bin("c:\\serialized.dat");
		//std::ifstream ifs_xml("c:\\serialized.xml");
        boost::archive::text_iarchive ia_txt(ifs_txt);
		//boost::archive::binary_iarchive ia_bin(ifs_bin);
		//boost::archive::xml_iarchive ia_xml(ifs_xml);
        // read class state from archive
        ia_txt >> newg;
		//ia_bin >> newg;
		//ia_xml >> newg;
        // archive and stream closed when destructors are called
    }

	system("PAUSE");
    return 0;
}
*/

/* // Boost State Chart Example

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/mpl/list.hpp>
#include <iostream>

using namespace std;

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

struct EvPressButton : sc::event<EvPressButton>
{
  EvPressButton()
  {
    cout << "Button is pressed" << endl;
  }
};

struct EvReleaseButton : sc::event<EvReleaseButton>
{
  EvReleaseButton()
  {
    cout << "Button is released" << endl;
  }
};

struct EvToggleLight : sc::event<EvToggleLight> {};

struct Active;
struct Switch : sc::state_machine<Switch, Active> {};

struct ButtonReleased;
struct ButtonPressed;
struct LightOff;
struct LightOn;

struct Active: sc::simple_state<Active, Switch,mpl::list<ButtonReleased, LightOff> > {};
struct ButtonPressed : sc::state<ButtonPressed, Active::orthogonal<0> >
{
  typedef sc::transition<EvReleaseButton, ButtonReleased> reactions;
  ButtonPressed(my_context ctx) : my_base(ctx)
  {
    post_event(EvToggleLight());
  }
};

struct ButtonReleased : sc::simple_state<ButtonReleased, Active::orthogonal<0> >
{
  typedef sc::transition<EvPressButton, ButtonPressed> reactions;
};

struct LightOff : sc::simple_state<LightOff, Active::orthogonal<1> >
{
  typedef sc::transition<EvToggleLight, LightOn> reactions;
  LightOff()
  {
    cout << "Light is off" << endl;
  }
};

struct LightOn : sc::simple_state<LightOn, Active::orthogonal<1> >
{
  typedef sc::transition<EvToggleLight, LightOff> reactions;
  LightOn()
  {
    cout << "Light is on" << endl;
  }
};

int main()
{
  Switch sw;
  sw.initiate();
  for (int i = 0; i < 5; i++)
  {
    sw.process_event(EvPressButton());
    sw.process_event(EvReleaseButton());
  }

  system("PAUSE");
  return 0;
}
*/

