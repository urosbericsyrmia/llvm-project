; RUN: llc %s -o %t1
; RUN: FileCheck %s < %t1

; CHECK:        test:
; CHECK-NOT:    retq
; CHECK:        movl	%[[OPERAND:[a-z]+]], -4(%rsp)
; CHECK-NEXT:   imull $2, %[[OPERAND]], %{{[a-z]+}}
; CHECK-NEXT:   retq

define i32 @test(i32 %0) {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = add nsw i32 %3, %4
  ret i32 %5
}