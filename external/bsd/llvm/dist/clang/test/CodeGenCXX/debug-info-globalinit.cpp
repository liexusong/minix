// RUN: %clang_cc1 -triple x86_64-apple-darwin10.0.0 -emit-llvm -o - %s -std=c++11 -g | FileCheck %s

void crash() {
  volatile char *ptr = 0;
  char x = *ptr;
}

int test() {
  crash();
  return 1;
}

static int i = test();
__attribute__((nodebug)) static int j = test();
static int k = test();

int main(void) {}

// CHECK-LABEL: define internal void @__cxx_global_var_init()
// CHECK-NOT: __cxx_global_var_init
// CHECK: %[[C0:.+]] = call i32 @_Z4testv(), !dbg ![[LINE:.*]]
// CHECK-NOT: __cxx_global_var_init
// CHECK: store i32 %[[C0]], i32* @_ZL1i, align 4, !dbg
// 
// CHECK-LABEL: define internal void @__cxx_global_var_init1()
// CHECK-NOT: dbg
// CHECK: %[[C1:.+]] = call i32 @_Z4testv()
// CHECK-NOT: dbg
// CHECK: store i32 %[[C1]], i32* @_ZL1j, align 4
//
// CHECK-LABEL: define internal void @__cxx_global_var_init2()
// CHECK-NOT: __cxx_global_var_init
// CHECK: %[[C2:.+]] = call i32 @_Z4testv(), !dbg ![[LINE2:.*]]
// CHECK-NOT: __cxx_global_var_init
// CHECK: store i32 %[[C2]], i32* @_ZL1k, align 4, !dbg
// 
// CHECK: ![[LINE]] = metadata !{i32 13, i32
// CHECK: ![[LINE2]] = metadata !{i32 15, i32
