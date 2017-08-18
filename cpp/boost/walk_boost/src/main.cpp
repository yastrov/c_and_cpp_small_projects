/*
 * Compile
 * g++ -std=c++11 -I"C:\\MinGW_nuwen64\\include\\" -Wall -c "%f"
 * g++ -std=c++11 -Wall -lboost_system -o "%e" "%f"
 * >walk_boost -f G:\\WWW_New\\  -p .zip -P
 * */
#include <cstdlib> // size_t, EXIT_FAILURE
#include <iostream>
#include <functional> // std::function
#include <vector>
#include <regex>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace bf = boost::filesystem;
namespace bo = boost::program_options;

void
walk_dirs(const std::vector<std::string>& dirs,
          std::function<void(const bf::path&)> func)
{
    //using namespace boost::filesystem;
    //size_t n_files(0);
    for (const auto& dir : dirs) {
        try {
            auto walker = bf::recursive_directory_iterator(bf::path(dir));
            for (const auto& entry : walker) {
                if (bf::is_regular_file(entry)) {
                    //n_files += 1;
                    func(entry.path());
                } else if(bf::is_directory(entry)) {
                    ;
                }
            }
        }
        catch (const bf::filesystem_error& ex) {
            std::cerr << "Error accessing " << dir << "\n\t" << ex.what() << std::endl;
        }
        catch (const std::exception& x) {
            std::cerr << "Error " << dir << "\n\t" << x.what() << std::endl;
        }
    }
}

int
main(int argc, char* argv[])
{
    std::function<void(const bf::path&)> task;
    std::vector<std::string> folders;
    std::string pattern;
    
    bo::options_description desc("General options");
    std::string task_mode = "extention";
    desc.add_options()
            ("help,h", "Show help")
            ("folders,f",  bo::value<std::vector<std::string> >(), "Folders")
            ("pattern,p", bo::value<std::string>(), "Pattern")
            ("mode,m", bo::value<std::string>(&task_mode), "Select mode: filename (Use regex for full filename), extention (Need point. Like '.zip')")
            ;
    bo::options_description task_desc("Task");
    task_desc.add_options()
            ("remove,r", "Remove files that find")
            ("print,P", "Print filenames")
            ;
    desc.add(task_desc);
    bo::variables_map vm;
    try {
        bo::parsed_options parsed = bo::command_line_parser(argc, argv)
                .options(desc)
                .allow_unregistered().run();
        bo::store(parsed, vm);
        bo::notify(vm);
        if(vm.count("folders")) {
            folders = vm["folders"].as<std::vector<std::string> >();
        } else {
            std::cout << desc << std::endl;
            std::cout << std::endl << "Need folders!" << std::endl;
            return EXIT_SUCCESS;
        }
        
        if(vm.count("pattern")) {
            pattern = vm["pattern"].as<std::string>();
        } else {
            std::cout << desc << std::endl;
            std::cout << std::endl << "Need pattern!" << std::endl;
            return EXIT_SUCCESS;
        }
        //Bool flags
        if(vm.count("remove") && vm.count("print")) {
            task = [](const bf::path& path) {
                if(bf::exists(path)) {
#ifdef BOOST_WINDOWS_API
                    std::wcout << L"Remove: " << path.native() << std::endl;
#else
                    std::cout << "Remove: " << path.native() << std::endl;
#endif
                    bf::remove(path);
                }
            };
        } else {
            if(vm.count("remove")) {
                task = [](const bf::path& path) {
                    if(bf::exists(path))
                        bf::remove(path);
                };
            } else {
                task = [](const bf::path& path) {
#ifdef BOOST_WINDOWS_API
                    std::wcout << path.native() << std::endl;
#else
                    std::cout << path.native() << std::endl;
#endif
                };
            }
        }
        
        if(task_mode == "extention") {
            walk_dirs(folders,
                      [&pattern, &task](const bf::path& path) {
                std::string ext = path.extension().string();
                if(ext == pattern) {
                    task(path);
                }
            });
        }
        else if(task_mode == "filename") {
            const std::regex txt_regex(pattern);
            walk_dirs(folders,
                      [&txt_regex, &task](const bf::path& path) {
                std::string fname = path.filename().string();
                if(std::regex_match(fname, txt_regex)) {
                    task(path);
                }
            });
        }
        else {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }
    }
    catch (const bf::filesystem_error& ex) {
        std::cerr << ex.what() << std::endl;
    }
    catch(const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
