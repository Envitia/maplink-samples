#include <tslmapbuildproject.h>

#include <cassert>
#include <filesystem>
#include <iostream>

static char const usage[] = "Usage: ASCIIMapGenerator output-project.mlp [output-map.map]\n";

int main(int argc, char* argv[])
{
  if (argc < 2 || argc > 3)
  {
    std::cerr << "Error: incorrect number of arguments.\n" << usage;
    return 1;
  }
  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
  {
    std::cout << usage;
    return 0;
  }

  std::filesystem::path src = __FILE__;
  auto samples = src.parent_path();
  auto install = samples.parent_path();
  auto msgs = install / "config/tslmapbuilderrors.msg";
  //tgm::Dbg::loadMessagesFile(msgs.string());

  envitia::MapLinkProject project;
  bool created = project.create();
  //errorCheck();
  assert(created);

  auto fbs1 = project.createFeatureBookSection("ASCII");
  //errorCheck();
  assert(fbs1);

  auto layer1 = project.newLayer("Layer1");
  //errorCheck();
  assert(layer1);

  // Load filter
  std::string fname = "ASCII";
  auto filter = envitia::loadFilter(fname);
  //errorCheck();
  assert(filter);
    
  // Specify dataset name - ASCII
  std::string datasetname = "ASCII";
  auto dataset1 = project.createDataset(layer1, filter, fbs1, &datasetname);
  //errorCheck();
  assert(dataset1);

  // Load dataset file
  std::filesystem::path const datapath = samples / "lat-long-30.ascii";
  auto data1 = project.addData(dataset1, datapath);
  //errorCheck();
  assert(data1);

  bool saved = project.save(argv[1]);
  //errorCheck();
  assert(saved);

  if (!argv[2])
    return 0;

  // Otput map options
  envitia::OutputMapOptions options = {
    /*.version           =*/ (TSLMapLinkVersion)15, //TSL_CURRENT_MAPLINK_VERSION,
    /*.saveProject       =*/ false,
    /*.mergeLayers       =*/ false
  };
  auto result = !project.CreateMap(argv[2], std::move(options));
  //errorCheck();
  assert(result);
}
