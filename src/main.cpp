#include <stdio.h>
#include <errno.h>
#include <error.h>

#include "Command.h"
#include "Display.h"
#include "File.h"

using namespace huge;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: huge FILE\n");
		return 1;
	}
	huge::File file(argv[1]);
	if (!file.is_opened()) {
		error(1, errno, "Failed to open file %s", file.filename());
	}
	Display display;
	Formatter formatter;
	Huge huge;
	huge.display(&display).file(&file).formatter(&formatter);

	display.refresh(formatter, huge.file_view());
	while (true) {
		Command *c = Command::read_command();
		if (!c->execute(huge)) break;
		delete c;
		display.refresh(formatter, huge.file_view());
	}
	return 0;
}
