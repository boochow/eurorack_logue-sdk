SUBDIRS = va dg fm rs nz
PLATFORMS = prologue nutekt-digital minilogue-xd

.PHONY: all $(SUBDIRS)

all: $(PLATFORMS)

$(PLATFORMS):
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir PLATFORM=$@ clean; \
		$(MAKE) -C $$dir PLATFORM=$@ install; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
