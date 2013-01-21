#include <stdio.h>
#include <errno.h>
#include <error.h>

#include "Command.h"
#include "Display.h"
#include "File.h"
#include "IndexJob.h"

using namespace huge;

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: huge FILE\n");
		return 1;
	}
	try {
		huge::File file(argv[1]);
		IndexJob job(file);
		job.run();

		/*Display display;
		Formatter formatter;
		Huge huge;
		huge.display(&display).file(&file).formatter(&formatter);

		display.refresh(formatter, huge.file_view());
		while (true) {
			Command *c = Command::read_command();
			if (!c->execute(huge)) break;
			delete c;
			display.refresh(formatter, huge.file_view());
		}*/
	} catch (FileOpenException &e) {
		e.print();
	}
	return 0;
}
