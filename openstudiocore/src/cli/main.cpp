#include "RubyInterpreter.hpp"
#include "GC_Value.hpp"
#include "InitMacros.hxx"
#include "../../ruby/init_openstudio.hpp"
#include <embedded_files.hxx>

#include <iostream>

#ifndef _MSC_VER
#include <dlfcn.h>
#include <dirent.h>
#else
#include <windows.h>
#endif


extern "C" {
  void Init_EmbeddedScripting(void);
  INIT_DECLARATIONS;

  void Init_encdb(void);
  ENC_INIT_DECLARATIONS;

  void Init_transdb(void);
  TRANS_INIT_DECLARATIONS;

  void Init_generator(void);
  void Init_parser(void);
  void Init_Encoding(void);
  void Init_md5(void);
  void Init_rmd160(void);
  void Init_sha1(void);
  void Init_sha2(void);
}

std::vector<std::string> paths;
static RubyInterpreter rubyInterpreter(paths);//(paths);

int main(int argc, char *argv[])
{
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();

    swig::GC_VALUE::hash_id = rb_intern("hash");
    swig::GC_VALUE::lt_id = rb_intern("<");
    swig::GC_VALUE::gt_id = rb_intern(">");
    swig::GC_VALUE::eq_id = rb_intern("==");
    swig::GC_VALUE::le_id = rb_intern("<=");
    swig::GC_VALUE::ge_id = rb_intern(">=");

    swig::GC_VALUE::pos_id = rb_intern("+@");
    swig::GC_VALUE::neg_id = rb_intern("-@");
    swig::GC_VALUE::inv_id = rb_intern("~");

    swig::GC_VALUE::add_id = rb_intern("+");
    swig::GC_VALUE::sub_id = rb_intern("-");
    swig::GC_VALUE::mul_id = rb_intern("*");
    swig::GC_VALUE::div_id = rb_intern("/");
    swig::GC_VALUE::mod_id = rb_intern("%");

    swig::GC_VALUE::and_id = rb_intern("&");
    swig::GC_VALUE::or_id  = rb_intern("|");
    swig::GC_VALUE::xor_id = rb_intern("^");

    swig::GC_VALUE::lshift_id = rb_intern("<<");
    swig::GC_VALUE::rshift_id = rb_intern(">>");

    INIT_CALLS;

    Init_encdb();
    rb_provide("encdb.so");
    ENC_INIT_CALLS;

    Init_transdb();
    rb_provide("trans/transdb");
    TRANS_INIT_CALLS;

    Init_Encoding();

    init_openstudio_internal();

    Init_md5();
    rb_provide("digest/md5");
    Init_rmd160();
    rb_provide("digest/rmd160");
    Init_sha1();
    rb_provide("digest/sha1");
    Init_sha2();
    rb_provide("digest/sha2");

    Init_EmbeddedScripting();
  }

  // Need embedded_help for requiring files out of the embedded system
  auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");
  rubyInterpreter.evalString(embedded_extensions_string);

  // chop off the first argument which is the exe path/name
  ruby_set_argv(argc - 1,argv + 1);

  try{
    rubyInterpreter.evalString("begin \n (require 'openstudio_cli') \n rescue Exception => e \n puts \n puts \"Error: #{e.message}\" \n puts \"Backtrace:\n\t\" + e.backtrace.join(\"\\n\\t\") \n raise \n end");
  } catch (const std::exception& e){
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Exception: " << e.what() << std::endl; // endl will flush
    return 1;
  } catch (...){
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Unknown Exception" << std::endl; // endl will flush
    return 1;
  }
  rubyInterpreter.evalString(R"(STDOUT.flush)");
  std::cout << std::flush;
  return 0;
}

extern "C" {
  int rb_hasFile(const char *t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = std::string(":/ruby/2.2.0/") + std::string(t_filename) + ".rb";
    return embedded_files::hasFile(expandedName);
  }

  int rb_require_embedded(const char *t_filename) {
    std::string require_script = R"(require ')" + std::string(t_filename) + R"(')";
    rubyInterpreter.evalString(require_script);
    return 0;
  }
}
