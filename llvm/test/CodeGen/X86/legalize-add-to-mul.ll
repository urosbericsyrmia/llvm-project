; RUN: llc %s -print-after-all -debug -o - &> %t1
; RUN: FileCheck %s < %t1

; CHECK:        Legalizing: t{{[0-9]+}}: i32 = add t{{[0-9]+}}, t{{[0-9]+}}
; CHECK-NEXT:   Trying custom legalization
; CHECK-NEXT:   Creating constant: t{{[0-9]+}}: i32 = Constant<2>
; CHECK-NEXT:   Creating new node: t{{[0-9]+}}: i32 = mul t{{[0-9]+}}, Constant:i32<2>
; CHECK-NEXT:   Successfully custom legalized node
; CHECK-NEXT:       ... replacing: t{{[0-9]+}}: i32 = add t{{[0-9]+}}, t{{[0-9]+}}
; CHECK-NEXT:           with:      t{{[0-9]+}}: i32 = mul t{{[0-9]+}}, Constant:i32<2>

define i32 @test(i32 %0) {
  %res = add i32 %0, %0
  ret i32 %res
}