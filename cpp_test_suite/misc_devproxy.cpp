/* 
 * example of a client using the TANGO device api.
 */

#include <tango.h>
#include <assert.h>

#ifdef WIN32
#include <process.h>
#endif


using namespace Tango;
using namespace std;

int main(int argc, char **argv)
{
	DeviceProxy *device;
	
	if (argc != 3)
	{
		cout << "usage: " << argv[0] << " <device> <full ds name>" << endl;
		exit(-1);
	}

	string device_name = argv[1];
	string ds_name = argv[2];
	string admin_device("dserver/");
	admin_device = admin_device + ds_name;

	try 
	{
		device = new DeviceProxy(device_name);
	}
	catch (CORBA::Exception &e)
	{
		Except::print_exception(e);
		exit(1);
	}

	cout << '\n' << "new DeviceProxy(" << device->name() << ") returned" << '\n' << endl;

	int elapsed;
	try
	{

// Test get_timeout

		int to;
		to = device->get_timeout_millis();
		
		assert( to == 3000 );
		cout << "   Get timeout --> OK" << endl;
		
// Test set_timeout

		int new_to;
		device->set_timeout_millis(2000);
		new_to = device->get_timeout_millis();
		
		assert ( new_to == 2000 );
		
		cout << "   Set timeout --> OK" << endl;
		
		device->set_timeout_millis(3000);
		
// Test ping
			
		elapsed = device->ping();

		cout << "   Ping ( " << elapsed << " us ) --> OK" << endl;
	
// Test state

		DevState sta;
		sta = device->state();

		assert ( sta == Tango::ON);
		cout << "   State --> OK" << endl;
	
// Test status

		string str;
		str = device->status();
	
		assert ( str == "The device is in ON state.");
		cout << "   Status --> OK" << endl;

// Test adm_name

		string str_adm;
		str_adm = device->adm_name();

		transform(str_adm.begin(),str_adm.end(),str_adm.begin(),::tolower);
		transform(admin_device.begin(),admin_device.end(),admin_device.begin(),::tolower);

		assert ( str_adm == admin_device);
		cout << "   Adm_name --> OK" << endl;

// Test description

		string desc;
		desc = device->description();

		assert ( desc == "A TANGO device");
		cout << "   Description --> OK" << endl;
		
// Test name

		string name;
		name = device->name();
	
		assert ( name == device_name);
		cout << "   Name --> OK" << endl;

// Test blackbox

		vector<string> *ptr;
		ptr = device->black_box(3);
		
		assert ( ptr->size() == 3 );
		
		string tmp = (*ptr)[0];
		string::size_type pos,end;
		pos = tmp.find('A');
		end = tmp.find("from");
		string ans = tmp.substr(pos,end - pos);
		assert ( ans == "Attribute name requested " );
		
		tmp = (*ptr)[1];
		end = tmp.find("from");
		ans = tmp.substr(pos,end - pos);
		assert ( ans == "Attribute description requested ");
		
		tmp = (*ptr)[2];
		end = tmp.find("from");
		ans = tmp.substr(pos,end - pos);
		assert ( ans == "Attribute adm_name requested ");		

		cout << "   Black box --> OK" << endl;
		delete ptr;
		
// Test info

		DeviceInfo inf;
		inf = device->info();
		assert( inf.dev_class == "DevTest" );
		assert( inf.server_id == ds_name );
		assert( inf.doc_url == "Doc URL = http://www.tango-controls.org" );
		assert( inf.dev_type == "TestDevice");
		
		cout << "   Info --> OK" << endl;
		
// Test command_query

		DevCommandInfo cmd_info;
		cmd_info = device->command_query("IODoubleArray");
		
		assert( cmd_info.cmd_name == "IODoubleArray" );
		assert( cmd_info.in_type == DEVVAR_DOUBLEARRAY );
		assert( cmd_info.out_type == DEVVAR_DOUBLEARRAY );
		assert( cmd_info.in_type_desc == "Array of double" );
		assert( cmd_info.out_type_desc == "This array * 2" );
		
		cout << "   Command_query --> OK" << endl;
		
// Test command_list_query

		CommandInfoList *cmd_list;
		cmd_list = device->command_list_query();
		cout << "cmd list size = " << cmd_list->size() << endl;
		
//		assert (cmd_list->size() == 88 );
//		assert ((*cmd_list)[0].cmd_name == "FileDb" );
//		assert ((*cmd_list)[87].cmd_name == "Status");

//		cout << "   Command_list_query --> OK" << endl;
		delete cmd_list;
		
// Test get_attribute_list

		vector<string> *att_list;
		att_list = device->get_attribute_list();

		cout << "att_list size = " << att_list->size() << endl;
//		assert ( att_list->size() == 77 );
//		assert ( (*att_list)[0] == "Short_attr");
//		assert ( (*att_list)[1] == "Long_attr");
//		assert ( (*att_list)[21] == "String_attr_w");
		
//		cout << "   Get attribute list --> OK" << endl;
		delete att_list;
		
// Test attribute query

		DeviceAttributeConfig attr_conf;
		attr_conf = device->attribute_query("Short_attr");
		
		assert ( attr_conf.name == "Short_attr" );
		assert ( attr_conf.data_format == SCALAR );
		assert ( attr_conf.data_type == DEV_SHORT );
		assert ( attr_conf.description == "No description" );
		assert ( attr_conf.max_dim_x == 1 );
		
		cout << "   Attribute config --> OK" << endl;

// Test get_attribute_config
		
		AttributeInfoList *attr_conf_ptr;
		vector<string> li;
		li.push_back("Long_attr");
		li.push_back("Double_attr");
		
		attr_conf_ptr = device->get_attribute_config(li);

		assert ( attr_conf_ptr->size() == 2 );
		
		assert ( (*attr_conf_ptr)[0].name == "Long_attr" );
		assert ( (*attr_conf_ptr)[0].data_format == SCALAR );
		assert ( (*attr_conf_ptr)[0].data_type == DEV_LONG );

		assert ( (*attr_conf_ptr)[1].name == "Double_attr" );
		assert ( (*attr_conf_ptr)[1].data_format == SCALAR );
		assert ( (*attr_conf_ptr)[1].data_type == DEV_DOUBLE );	
		
		cout << "   Get attribute config --> OK" << endl;
		delete attr_conf_ptr;

// test attribute_list_query

//		AttributeInfoList *attr_confs;
//		attr_confs = device->attribute_list_query();
		
//		assert ( attr_confs->size() == 77 );
//		assert ( (*attr_confs)[0].name == "Short_attr");
//		assert ( (*attr_confs)[0].data_format == SCALAR);
//		assert ( (*attr_confs)[0].data_type == DEV_SHORT);

//		assert ( (*attr_confs)[1].name == "Long_attr");
//		assert ( (*attr_confs)[1].data_type == DEV_LONG);
//		assert ( (*attr_confs)[1].data_format == SCALAR);

//		assert ( (*attr_confs)[21].name == "String_attr_w");				
//		assert ( (*attr_confs)[21].data_type == DEV_STRING);
//		assert ( (*attr_confs)[21].data_format == SCALAR);
		
//		cout << "   Attribute list query --> OK "  << endl;
//		delete attr_confs;

// Test set_attribute_config

		AttributeInfoList v_conf;
		AttributeInfo co = device->attribute_query("Short_attr");

#ifdef WIN32
		int pid = _getpid();
#else
		pid_t pid = getpid();
#endif

#if ((defined WIN32) || (defined __SUNPRO_CC) || (defined GCC_STD))
		stringstream st;
#else
		strstream st;
#endif
		string s;
		st << pid;
		st >> s;
		
		co.format = s;
		v_conf.push_back(co);
		
		device->set_attribute_config(v_conf);
		
		DeviceAttributeConfig res = device->attribute_query("Short_attr");
		assert ( res.format == s );
		
		cout << "   Set attribute config --> OK" << endl;
		
// Test device version

#ifndef COMPAT
		int vers = device->get_idl_version();
		assert (vers == 4);
		
		cout << "   Get IDL version --> OK" << endl;
#endif
		
// Test source 

		Tango::DevSource so = device->get_source();
		assert( so == Tango::CACHE_DEV );
		
		device->set_source(Tango::DEV);
		assert (device->get_source() == Tango::DEV);
		
		device->set_source(Tango::CACHE_DEV);
		
		cout << "   Source parameter --> OK" << endl;

// Test get property list

#ifndef COMPAT
		vector<string> props;
		device->get_property_list("*",props);

		cout << "NB prop = " << props.size() << endl;
		for (unsigned long l = 0;l < props.size();l++)
			cout << "prop = " << props[l] << endl;
		assert (props.size() == 8);
		assert (props[0] == "cmd_min_poll_period");
		assert (props[1] == "logging_level");
		assert (props[2] == "logging_rft");
		assert (props[3] == "logging_target");
		assert (props[4] == "min_poll_period");
		assert (props[5] == "poll_old_factor");
		assert (props[6] == "tst_property");
		assert (props[7] == "__SubDevices");

		cout << "   Get property list --> OK" << endl;
#endif
	}
	catch (Tango::DevFailed &e)
	{
		Except::print_exception(e);
		exit(-1);
	}
		
	delete device;
	
	return 0;
}
