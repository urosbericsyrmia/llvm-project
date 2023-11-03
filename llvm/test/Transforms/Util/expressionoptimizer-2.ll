; RUN: opt -S -passes=expressionoptimizer %s >&1 | FileCheck %s

; CHECK: {{.*}}main(){{.*}}
; CHECK-NEXT: %1 = alloca i32, align 4
; CHECK-NEXT: %2 = alloca i32, align 4
; CHECK-NEXT: %3 = alloca i32, align 4
; CHECK-NEXT: %4 = alloca i32, align 4
; CHECK-NEXT: store i32 0, ptr %1, align 4
; CHECK-NEXT: store i32 2, ptr %2, align 4
; CHECK-NEXT: store i32 3, ptr %3, align 4
; CHECK-NEXT: %5 = load i32, ptr %3, align 4
; CHECK-NEXT: %6 = load i32, ptr %2, align 4
; CHECK-NEXT: %7 = add nsw i32 %5, %6
; CHECK-NEXT: %8 = mul nsw i32 %7, %7
; CHECK-NEXT: store i32 %8, ptr %4, align 4
; CHECK-NEXT: ret i32 0

; ModuleID = 'main.cpp'
source_filename = "main.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline norecurse nounwind uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 2, i32* %2, align 4
  store i32 3, i32* %3, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %3, align 4
  %7 = mul nsw i32 %5, %6
  %8 = load i32, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = mul nsw i32 %8, %9
  %11 = add nsw i32 %7, %10
  %12 = load i32, i32* %2, align 4
  %13 = mul nsw i32 %12, 2
  %14 = load i32, i32* %3, align 4
  %15 = mul nsw i32 %13, %14
  %16 = add nsw i32 %11, %15
  store i32 %16, i32* %4, align 4
  ret i32 0
}

attributes #0 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}