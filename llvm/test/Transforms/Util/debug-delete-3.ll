; RUN: opt -S -passes=debugdelete %s -o %t1
; RUN: opt -disable-output -passes=debugcount %t1 2>&1 | FileCheck %s

; ModuleID = 'main.cpp'
source_filename = "main.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; CHECK: Function: {{.*}}main{{.*}}
; CHECK-NEXT: llvm.dbg.declare: 0
; CHECK-NEXT: llvm.dbg.value: 0
; CHECK-NEXT: llvm.dbg.label: 0
; CHECK-NEXT: llvm.dbg.assign: 0
; Function Attrs: noinline norecurse nounwind uwtable
define dso_local i32 @main() #0 !dbg !7 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !11, metadata !DIExpression()), !dbg !12
  store i32 2, i32* %2, align 4, !dbg !12
  call void @llvm.dbg.declare(metadata i32* %3, metadata !13, metadata !DIExpression()), !dbg !14
  store i32 3, i32* %3, align 4, !dbg !14
  call void @llvm.dbg.declare(metadata i32* %4, metadata !15, metadata !DIExpression()), !dbg !16
  %5 = load i32, i32* %2, align 4, !dbg !17
  %6 = load i32, i32* %3, align 4, !dbg !18
  %7 = sdiv i32 %6, 2, !dbg !19
  %8 = add nsw i32 %5, %7, !dbg !20
  store i32 %8, i32* %4, align 4, !dbg !21
  call void @llvm.dbg.value(metadata i32 2, metadata !23, metadata !DIExpression()), !dbg !25
  call void @llvm.dbg.value(metadata i32 3, metadata !24, metadata !DIExpression()), !dbg !25
  ret i32 0, !dbg !22
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

attributes #0 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "clang version 10.0.0-4ubuntu1 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "main.cpp", directory: "/home/uberic@syrmia.com/Desktop/llvm/examples")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 10.0.0-4ubuntu1 "}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "a", scope: !7, file: !1, line: 2, type: !10)
!12 = !DILocation(line: 2, column: 6, scope: !7)
!13 = !DILocalVariable(name: "b", scope: !7, file: !1, line: 3, type: !10)
!14 = !DILocation(line: 3, column: 6, scope: !7)
!15 = !DILocalVariable(name: "c", scope: !7, file: !1, line: 4, type: !10)
!16 = !DILocation(line: 4, column: 6, scope: !7)
!17 = !DILocation(line: 5, column: 6, scope: !7)
!18 = !DILocation(line: 5, column: 10, scope: !7)
!19 = !DILocation(line: 5, column: 12, scope: !7)
!20 = !DILocation(line: 5, column: 8, scope: !7)
!21 = !DILocation(line: 5, column: 4, scope: !7)
!22 = !DILocation(line: 6, column: 2, scope: !7)
!23 = !DILocalVariable(name: "a", scope: !7, file: !1, line: 2, type: !10)
!24 = !DILocalVariable(name: "b", scope: !7, file: !1, line: 3, type: !10)
!25 = !DILocation(line: 0, scope: !7)