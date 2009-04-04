
lib_LTLIBRARIES = libjit.la

libjit_la_SOURCES = \
		jit-alloc.c \
		jit-apply.c \
		jit-apply-func.h \
		jit-apply-alpha.h \
		jit-apply-alpha.c \
		jit-apply-arm.h \
		jit-apply-arm.c \
		jit-apply-x86.h \
		jit-apply-x86.c \
		jit-apply-x86-64.h \
		jit-apply-x86-64.c \
		jit-bitset.h \
		jit-bitset.c \
		jit-block.c \
		jit-cache.h \
		jit-cache.c \
		jit-cfg.h \
		jit-cfg.c \
		jit-context.c \
		jit-cpuid-x86.h \
		jit-cpuid-x86.c \
		jit-debugger.c \
		jit-dump.c \
		jit-elf-defs.h \
		jit-elf-read.c \
		jit-elf-write.c \
		jit-except.c \
		jit-function.c \
		jit-gen-alpha.h \
		jit-gen-arm.h \
		jit-gen-arm.c \
		jit-gen-x86.h \
		jit-gen-i486-simd.h \
		jit-gen-x86-64.h \
		jite-gen-i486-masm.h \
		jite-i486-extra-arith.h \
		jit-insn.c \
		jit-init.c \
		jit-internal.h \
		jit-interp.h \
		jit-interp.c \
		jit-intrinsic.c \
		jit-live.c \
		jite-linear-scan.c \
		jite-linear-scan.h \
		jite-linear-scan-i486.c \
		jite-linear-scan-i486.h \
		jit-memory.c \
		jit-memory.h \
		jit-meta.c \
		jit-objmodel.c \
		jit-opcode.c \
		jit-pool.c \
		jit-reg-alloc.h \
		jit-reg-alloc.c \
		jit-reg-class.h \
		jit-reg-class.c \
		jit-rules.h \
		jit-rules.c \
		jit-rules-interp.c \
		jit-rules-interp.h \
		jit-rules-alpha.h \
		jit-rules-alpha.c \
		jit-rules-arm.h \
		jit-rules-arm.c \
		jit-rules-x86.h \
		jit-rules-x86.c \
		jit-rules-x86-64.h \
		jit-rules-x86-64.c \
		jit-setjmp.h \
		jit-signal.c \
		jit-string.c \
		jit-symbol.c \
		jit-thread.c \
		jit-thread.h \
		jit-type.c \
		jit-unwind.c \
		jit-value.c \
		jit-walk.c

EXTRA_DIST = \
		mklabel.sh \
		jit-rules-alpha.ins \
		jit-rules-arm.sel \
		jit-rules-x86.ins \
		jit-rules-x86-64.ins \
		jite-rules-i486-arith.lir \
		jite-rules-i486-conv.lir \
		jite-rules-i486-obj.lir \
		jite-rules-i486-call.lir \
		jite-rules-i486-math.lir \
		jite-rules-i486-branch.lir \
		jite-rules-i486-except.lir \
		jite-rules-i486-logic.lir

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include -I. -I$(srcdir)

libjit_la_LDFLAGS = -version-info $(LIBJIT_VERSION) -no-undefined

jit-interp.lo: jit-interp-labels.h

jit-interp-labels.h: $(top_srcdir)/include/jit/jit-opcode.h \
		$(top_srcdir)/jit/jit-interp.h $(srcdir)/mklabel.sh
	$(SHELL) $(srcdir)/mklabel.sh "$(AWK)" \
		$(top_srcdir)/include/jit/jit-opcode.h \
		$(top_srcdir)/jit/jit-interp.h >jit-interp-labels.h

jite-linear-scan-i486.lo: jite-rules-i486-arith.inc jite-rules-i486-conv.inc jite-rules-i486-obj.inc jite-rules-i486-call.inc jite-rules-i486-math.inc jite-rules-i486-branch.inc jite-rules-i486-except.inc jite-rules-i486-logic.inc

jit-linear-scan.lo: jit-linear-scan-i486.lo

jit-rules-x86.lo: jit-rules-x86.inc

jit-rules-x86.inc: jit-rules-x86.ins $(top_builddir)/tools/gen-rules$(EXEEXT)
	$(top_builddir)/tools/gen-rules$(EXEEXT) $(srcdir)/jit-rules-x86.ins \
			>jit-rules-x86.inc

jite-rules-i486-arith.inc: jite-rules-i486-arith.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-arith.lir \
			>jite-rules-i486-arith.inc

jite-rules-i486-conv.inc: jite-rules-i486-conv.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-conv.lir \
			>jite-rules-i486-conv.inc

jite-rules-i486-obj.inc: jite-rules-i486-obj.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-obj.lir \
			>jite-rules-i486-obj.inc

jite-rules-i486-call.inc: jite-rules-i486-call.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-call.lir \
			>jite-rules-i486-call.inc

jite-rules-i486-math.inc: jite-rules-i486-math.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-math.lir \
			>jite-rules-i486-math.inc

jite-rules-i486-branch.inc: jite-rules-i486-branch.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-branch.lir \
			>jite-rules-i486-branch.inc

jite-rules-i486-except.inc: jite-rules-i486-except.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-except.lir \
			>jite-rules-i486-except.inc

jite-rules-i486-logic.inc: jite-rules-i486-logic.lir $(top_builddir)/tools/gen-lir$(EXEEXT)
	$(top_builddir)/tools/gen-lir$(EXEEXT) $(srcdir)/jite-rules-i486-logic.lir \
			>jite-rules-i486-logic.inc


jit-rules-arm.lo: jit-rules-arm.slc

jit-rules-arm.slc: jit-rules-arm.sel $(top_builddir)/tools/gen-sel$(EXEEXT)
	$(top_builddir)/tools/gen-sel$(EXEEXT) $(srcdir)/jit-rules-arm.sel \
			>jit-rules-arm.slc

jit-rules-alpha.lo: jit-rules-alpha.inc

jit-rules-alpha.inc: jit-rules-alpha.ins $(top_builddir)/tools/gen-rules$(EXEEXT)
	$(top_builddir)/tools/gen-rules$(EXEEXT) $(srcdir)/jit-rules-alpha.ins \
			>jit-rules-alpha.inc

jit-rules-x86-64.lo: jit-rules-x86-64.inc

jit-rules-x86-64.inc: jit-rules-x86-64.ins $(top_builddir)/tools/gen-rules$(EXEEXT)
	$(top_builddir)/tools/gen-rules$(EXEEXT) $(srcdir)/jit-rules-x86-64.ins \
			>jit-rules-x86-64.inc

CLEANFILES = \
	jit-interp-labels.h \
	jit-rules-alpha.inc \
	jit-rules-arm.slc \
	jit-rules-x86.inc \
	jit-rules-x86-64.inc \
	jite-rules-i486-arith.inc \
	jite-rules-i486-conv.inc \
	jite-rules-i486-obj.inc \
	jite-rules-i486-call.inc \
	jite-rules-i486-math.inc \
	jite-rules-i486-branch.inc \
	jite-rules-i486-except.inc \
	jite-rules-i486-logic.inc
