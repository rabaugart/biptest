//
// Starts mercurial (hg summary) and reads the revision number
//
#include <iostream>
#include <regex>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>

namespace po = boost::program_options;
namespace bfs = boost::filesystem;
namespace pr = boost::process;

int main( int argc, char** argv )
{
	std::string svn;
	po::options_description desc("Allowed options");
	desc.add_options()
	    ("help", "produce help message")
	    ("svn", po::value<std::string>(&svn)->default_value(""), "path to svn directory")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
	    std::cout << desc << "\n";
	    return 1;
	}

	bfs::current_path( svn );

	const std::vector<bfs::path> addPaths{"/usr/local/bin"};

	const std::vector<bfs::path> allPaths{ [&addPaths]() {
		std::vector<bfs::path> ret = boost::this_process::path();
		ret.insert( ret.end(), addPaths.begin(), addPaths.end() );
		return ret;
	}() };

	bfs::path hg = pr::search_path("hg",addPaths);
	if ( hg.empty() )
		throw std::runtime_error( "hg executable not found ");

	std::future<std::string> fdata;
	const int ret = pr::system( hg, "summary", pr::std_out > fdata );

	const std::string data = fdata.get();

	// parent: 791:28a023ad9c5c
	const std::regex re( "parent: [0-9]+:([0-9a-z]+)");

	std::smatch ma;

	const bool found = std::regex_search( data, ma, re );

	if ( ! found )
		throw std::runtime_error("Version pattern not found in hg output");

	const std::string rev = ma[1].str();

	std::cout << "Ok " << svn << " " << ret << std::endl << data << std::endl << rev << std::endl;

	return 0;
}
