//
// File generated by rootcint at Fri Jul  1 18:46:03 2022

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME MyClass_RootDict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "MyClass_RootDict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// Direct notice to TROOT of the dictionary's loading.
namespace {
   static struct DictInit {
      DictInit() {
         ROOT::RegisterModule();
      }
   } __TheDictionaryInitializer;
}

// START OF SHADOWS

namespace ROOTShadow {
   namespace Shadow {
      #if !(defined(R__ACCESS_IN_SYMBOL) || defined(R__USE_SHADOW_CLASS))
      typedef ::MyClass MyClass;
      #else
      class MyClass  {
         public:
         //friend XX;
         ::basic_stringstream< char, ::char_traits< char >, allocator< char >  > internal; //
      };
      #endif

   } // of namespace Shadow
} // of namespace ROOTShadow
// END OF SHADOWS

namespace ROOTDict {
   void MyClass_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void MyClass_Dictionary();
   static void *new_MyClass(void *p = 0);
   static void *newArray_MyClass(Long_t size, void *p);
   static void delete_MyClass(void *p);
   static void deleteArray_MyClass(void *p);
   static void destruct_MyClass(void *p);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::MyClass*)
   {
      // Make sure the shadow class has the right sizeof
      R__ASSERT(sizeof(::MyClass) == sizeof( ::ROOTShadow::Shadow::MyClass));
      ::MyClass *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::MyClass),0);
      static ::ROOT::TGenericClassInfo 
         instance("MyClass", "./MyClass.h", 11,
                  typeid(::MyClass), ::ROOT::DefineBehavior(ptr, ptr),
                  &MyClass_ShowMembers, &MyClass_Dictionary, isa_proxy, 4,
                  sizeof(::MyClass) );
      instance.SetNew(&new_MyClass);
      instance.SetNewArray(&newArray_MyClass);
      instance.SetDelete(&delete_MyClass);
      instance.SetDeleteArray(&deleteArray_MyClass);
      instance.SetDestructor(&destruct_MyClass);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::MyClass*)
   {
      return GenerateInitInstanceLocal((::MyClass*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::MyClass*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void MyClass_Dictionary() {
      ::ROOTDict::GenerateInitInstanceLocal((const ::MyClass*)0x0)->GetClass();
   }

} // end of namespace ROOTDict

//______________________________________________________________________________
namespace ROOTDict {
   void MyClass_ShowMembers(void *obj, TMemberInspector &R__insp)
   {
      // Inspect the data members of an object of class MyClass.
      typedef ::ROOTShadow::Shadow::MyClass ShadowClass;
      ShadowClass *sobj = (ShadowClass*)obj;
      if (sobj) { } // Dummy usage just in case there is no datamember.

      TClass *R__cl  = ::ROOTDict::GenerateInitInstanceLocal((const ::MyClass*)0x0)->GetClass();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "internal", (void*)&sobj->internal);
      R__insp.InspectMember("stringstream", (void*)&sobj->internal, "internal.", false);
   }

}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_MyClass(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::MyClass : new ::MyClass;
   }
   static void *newArray_MyClass(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::MyClass[nElements] : new ::MyClass[nElements];
   }
   // Wrapper around operator delete
   static void delete_MyClass(void *p) {
      delete ((::MyClass*)p);
   }
   static void deleteArray_MyClass(void *p) {
      delete [] ((::MyClass*)p);
   }
   static void destruct_MyClass(void *p) {
      typedef ::MyClass current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOTDict for class ::MyClass

/********************************************************
* MyClass_RootDict.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableMyClass_RootDict();

extern "C" void G__set_cpp_environmentMyClass_RootDict() {
  G__cpp_reset_tagtableMyClass_RootDict();
}
#include <new>
extern "C" int G__cpp_dllrevMyClass_RootDict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* MyClass */
static int G__MyClass_RootDict_168_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   MyClass* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new MyClass[n];
     } else {
       p = new((void*) gvp) MyClass[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new MyClass;
     } else {
       p = new((void*) gvp) MyClass;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   MyClass* p = NULL;
   char* gvp = (char*) G__getgvp();
   //m: 1
   if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
     p = new MyClass(*(MyClass*) libp->para[0].ref);
   } else {
     p = new((void*) gvp) MyClass(*(MyClass*) libp->para[0].ref);
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 103, (long) ((MyClass*) G__getstructoffset())->Print());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 103, (long) ((MyClass*) G__getstructoffset())->Set(*((string*) G__int(libp->para[0])), *((string*) G__int(libp->para[1]))));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 103, (long) ((MyClass*) G__getstructoffset())->Get(*((string*) G__int(libp->para[0])), *(string*) libp->para[1].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 103, (long) ((MyClass*) G__getstructoffset())->Set(*((string*) G__int(libp->para[0])), (int) G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__MyClass_RootDict_168_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 103, (long) ((MyClass*) G__getstructoffset())->Get(*((string*) G__int(libp->para[0])), *(int*) G__Intref(&libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef MyClass G__TMyClass;
static int G__MyClass_RootDict_168_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 0
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (MyClass*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((MyClass*) (soff+(sizeof(MyClass)*i)))->~G__TMyClass();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (MyClass*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((MyClass*) (soff))->~G__TMyClass();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* MyClass */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncMyClass_RootDict {
 public:
  G__Sizep2memfuncMyClass_RootDict(): p(&G__Sizep2memfuncMyClass_RootDict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncMyClass_RootDict::*p)();
};

size_t G__get_sizep2memfuncMyClass_RootDict()
{
  G__Sizep2memfuncMyClass_RootDict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceMyClass_RootDict() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableMyClass_RootDict() {

   /* Setting up typedef entry */
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* MyClass */
static void G__setup_memvarMyClass(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass));
   { MyClass *p; p=(MyClass*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->internal)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__MyClass_RootDictLN_basic_stringstreamlEcharcOchar_traitslEchargRcOallocatorlEchargRsPgR),G__defined_typename("stringstream"),-1,1,"internal=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarMyClass_RootDict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncMyClass(void) {
   /* MyClass */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass));
   G__memfunc_setup("MyClass",700,G__MyClass_RootDict_168_0_1, 105, G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("MyClass",700,G__MyClass_RootDict_168_0_2, 105, G__get_linked_tagnum(&G__MyClass_RootDictLN_MyClass), -1, 0, 1, 1, 1, 0, "u 'MyClass' - 11 - other", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Print",525,G__MyClass_RootDict_168_0_3, 103, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Set",300,G__MyClass_RootDict_168_0_4, 103, -1, -1, 0, 2, 1, 1, 0, 
"u 'string' - 0 - name u 'string' - 0 - in", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Get",288,G__MyClass_RootDict_168_0_5, 103, -1, -1, 0, 2, 1, 1, 0, 
"u 'string' - 0 - name u 'string' - 1 - out", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Set",300,G__MyClass_RootDict_168_0_6, 103, -1, -1, 0, 2, 1, 1, 0, 
"u 'string' - 0 - name i - - 0 - in", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Get",288,G__MyClass_RootDict_168_0_7, 103, -1, -1, 0, 2, 1, 1, 0, 
"u 'string' - 0 - name i - - 1 - out", (char*)NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~MyClass", 826, G__MyClass_RootDict_168_0_8, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncMyClass_RootDict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalMyClass_RootDict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {
}

static void G__cpp_setup_func12() {
}

static void G__cpp_setup_func13() {
}

static void G__cpp_setup_func14() {
}

static void G__cpp_setup_func15() {
}

static void G__cpp_setup_func16() {
}

static void G__cpp_setup_func17() {
}

static void G__cpp_setup_func18() {
}

static void G__cpp_setup_func19() {
}

static void G__cpp_setup_func20() {
}

static void G__cpp_setup_func21() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcMyClass_RootDict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
  G__cpp_setup_func12();
  G__cpp_setup_func13();
  G__cpp_setup_func14();
  G__cpp_setup_func15();
  G__cpp_setup_func16();
  G__cpp_setup_func17();
  G__cpp_setup_func18();
  G__cpp_setup_func19();
  G__cpp_setup_func20();
  G__cpp_setup_func21();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__MyClass_RootDictLN_string = { "string" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_basic_stringstreamlEcharcOchar_traitslEchargRcOallocatorlEchargRsPgR = { "basic_stringstream<char,char_traits<char>,allocator<char> >" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__MyClass_RootDictLN_MyClass = { "MyClass" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableMyClass_RootDict() {
  G__MyClass_RootDictLN_string.tagnum = -1 ;
  G__MyClass_RootDictLN_basic_stringstreamlEcharcOchar_traitslEchargRcOallocatorlEchargRsPgR.tagnum = -1 ;
  G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__MyClass_RootDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__MyClass_RootDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__MyClass_RootDictLN_MyClass.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableMyClass_RootDict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_string);
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_basic_stringstreamlEcharcOchar_traitslEchargRcOallocatorlEchargRsPgR);
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__MyClass_RootDictLN_MyClass),sizeof(MyClass),-1,262912,(char*)NULL,G__setup_memvarMyClass,G__setup_memfuncMyClass);
}
extern "C" void G__cpp_setupMyClass_RootDict(void) {
  G__check_setup_version(30051515,"G__cpp_setupMyClass_RootDict()");
  G__set_cpp_environmentMyClass_RootDict();
  G__cpp_setup_tagtableMyClass_RootDict();

  G__cpp_setup_inheritanceMyClass_RootDict();

  G__cpp_setup_typetableMyClass_RootDict();

  G__cpp_setup_memvarMyClass_RootDict();

  G__cpp_setup_memfuncMyClass_RootDict();
  G__cpp_setup_globalMyClass_RootDict();
  G__cpp_setup_funcMyClass_RootDict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncMyClass_RootDict();
  return;
}
class G__cpp_setup_initMyClass_RootDict {
  public:
    G__cpp_setup_initMyClass_RootDict() { G__add_setup_func("MyClass_RootDict",(G__incsetup)(&G__cpp_setupMyClass_RootDict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initMyClass_RootDict() { G__remove_setup_func("MyClass_RootDict"); }
};
G__cpp_setup_initMyClass_RootDict G__cpp_setup_initializerMyClass_RootDict;

