CC = clang
CFLAGS = -g -Wall -Werror -std=c11
LDFLAGS = -lm

DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

TESTS = rope_test \
		rbtree_test \

all: $(TESTS)

%_test : %.o %_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
		$(COMPILE.c) $(OUTPUT_OPTION) $< 
				$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))

clean:
	rm -rf *.o
	rm -rf $(DEPDIR)

