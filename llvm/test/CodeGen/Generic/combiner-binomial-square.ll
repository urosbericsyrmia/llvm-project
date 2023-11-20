; RUN: llc %s -o %t1
; RUN: FileCheck %s < %t1

; CHECK:        # %bb.0
; CHECK-NOT:        retq
; CHECK:            movl	%e[[OPERAND1:[a-z]+]], -4(%rsp)
; CHECK-NEXT:	    movl	%e[[OPERAND2:[a-z]+]], -8(%rsp)
; CHECK-NEXT:	    leal	(%r[[OPERAND2]],%r[[OPERAND1]]), %e[[ADDITION:[a-z]+]]
; CHECK-NEXT:	    imull	%e[[ADDITION]], %e[[ADDITION]]
; CHECK-NEXT:	    retq

define i32 @test(i32 %0, i32 %1) {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, i32* %3, align 4
  store i32 %1, i32* %4, align 4
  %5 = load i32, i32* %3, align 4
  %6 = load i32, i32* %3, align 4
  %7 = mul nsw i32 %5, %6
  %8 = load i32, i32* %3, align 4
  %9 = mul nsw i32 2, %8
  %10 = load i32, i32* %4, align 4
  %11 = mul nsw i32 %9, %10
  %12 = add nsw i32 %7, %11
  %13 = load i32, i32* %4, align 4
  %14 = load i32, i32* %4, align 4
  %15 = mul nsw i32 %13, %14
  %16 = add nsw i32 %12, %15
  ret i32 %16
}