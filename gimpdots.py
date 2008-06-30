import math
from gimpfu import *

def python_blurdots(timg, tdrawable, count):
	h = 2 * count * 3
	w = 2 * count * count
	img = gimp.image(w, h, RGB)
	gimp.display(img)
	

gimp.install_temp_proc(
	"python_fu_blurdots",
	"Create three rows of blurred dots with increasing sizes",
	"Create three rows of blurred dots with increasing sizes",
	"Rian Shelley",
	"Rian Shelley",
	"2008",
	"<Image>/Filters/Artistic/_Blurdots...",
	"RGB*",
	[
		(PF_INT, "count", "Number of dots", 16),
	],
	[],
	python_blurdots)


