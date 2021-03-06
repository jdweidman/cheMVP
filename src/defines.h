#ifndef DEFINES_H_
#define DEFINES_H_

#define COMPANY_NAME "CCQC"
#define COMPANY_DOMAIN "ccqc.uga.edu"
#define PROGRAM_NAME "cheMVP"
#define CHEMVP_VERSION "0.2"

// These are the types used in the enums to distinguish drawing types
#define ATOMTYPE 1
#define BONDTYPE 2
#define ANGLETYPE 3
#define ANGLEMARKERTYPE 4
#define ARROWTYPE 5
#define DRAGBOXTYPE 6
#define ANGLELABELTYPE 7
#define BONDLABELTYPE 8
#define TEXTLABELTYPE 9

#define TINY 0.00001

#define EXTRA_DRAWING_SPACE 0.2

#define DEFAULT_LABEL_FONT "Helvetica"
#define DEFAULT_LABEL_FONT_SIZE 12

#define DEFAULT_ATOM_LABEL_FONT "Helvetica"
#define DEFAULT_ATOM_LABEL_FONT_SIZE 14
#define DEFAULT_LARGE_ATOM_LABEL_FONT_SIZE 22

#define DEFAULT_FOGGING_SCALE 60
#define DEFAULT_PERSPECTIVE_SCALE 50.0
#define DEFAULT_BACKGROUND_OPACITY 100

#define DEFAULT_SCENE_SIZE_X 750.0
#define DEFAULT_SCENE_SIZE_Y 450.0
#define DEFAULT_TOOLBOX_WIDTH 350.0
#define DZ_ZERO_TOL 0.01
#define SELECTED_COLOR QColor(104, 250, 14, 150)

#define DEFAULT_BOND_LABEL_PRECISION 3
#define DEFAULT_ANGLE_LABEL_PRECISION 1

#define DEFAULT_ATOM_SCALE_FACTOR 0.55
#define DEFAULT_BOND_THICKNESS 0.2
#define DEFAULT_DASHED_BOND_THICKNESS 0.05

#define DEFAULT_ARROW_THICKNESS 0.013

#define BOHR_TO_ANG 0.529177249
#define ANG_TO_BOHR 1.889725989
#define DEG_TO_RAD 0.01745329251994330
#define RAD_TO_DEG 57.29577951308232
#define PI 3.14159265

#define ANGLE_MARKER_OFFSET 0.4

#define ITEM_IS_LABEL                                                                              \
    item->type() == Label::AngleLabelType || item->type() == Label::BondLabelType ||               \
        item->type() == Label::TextLabelType

#define MAX_RECENT_FILES 5

#endif /*DEFINES_H_*/
