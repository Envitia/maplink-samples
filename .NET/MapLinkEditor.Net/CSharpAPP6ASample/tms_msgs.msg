# TMS Editor Messages File 
# ------------------------
# This file contains prompts for the user and is not releated to the Error Stack.

# Categories 

0	TMS_MSG_INFO   
1	TMS_MSG_WARN  
2	TMS_MSG_ERROR
3	TMS_MSG_FATAL  
4	TMS_MSG_SYNTX 
5	TMS_MSG_DEBUG  

# Messages

0 AP_INST		TMS_MSG_INFO   	Select position for instance
1 AP_SELECT		TMS_MSG_INFO 	Select object or operation
2 AP_SELAREA		TMS_MSG_INFO	Select start of area
3 LP_SELAREA		TMS_MSG_INFO	Select next point of area
4 AP_SELEXT		TMS_MSG_INFO 	Select first corner of extent
5 LP_SELEXT		TMS_MSG_INFO 	Select opposite corner
6 AP_MOVE		TMS_MSG_INFO 	Select new position
7 AP_RECT		TMS_MSG_INFO 	Select first corner of rect
8 LP_RECT		TMS_MSG_INFO 	Select opposite corner of rect
9 AP_POINT		TMS_MSG_INFO 	Select position for point
10 AP_POLY		TMS_MSG_INFO 	Select start of polyline
11 LP_POLY		TMS_MSG_INFO 	Select next point of polyline
12 AP_CIRC		TMS_MSG_INFO 	Select centre of circle
13 LP_CIRC		TMS_MSG_INFO 	Select point on radius
14 AP_TEXT		TMS_MSG_INFO 	Select position for text
15 LP_TEXT		TMS_MSG_INFO	Enter text string 
16 AP_COPY		TMS_MSG_INFO 	Select position for clone
17 AP_SCALE		TMS_MSG_INFO 	Select new size
18 AP_TEXTREPL		TMS_MSG_INFO 	Enter new text
19 AE_STEXT_NO_TEXT	TMS_MSG_ERROR	No text objects selected
20 AE_XFRM_NOT_DEEP	TMS_MSG_ERROR 	Cannot transform deep objects
21 AE_DEL_NOT_DEEP	TMS_MSG_ERROR 	Cannot delete deep objects
22 AE_PNT_NO_PNTS	TMS_MSG_ERROR	Invalid object for Point operation
23 LE_ADDPNT_ADJ	TMS_MSG_ERROR	Selected points must be an endpoint or adjacent to each other
24 AP_ADDPNT		TMS_MSG_INFO	Select first point to add between
25 LP1_ADDPNT		TMS_MSG_INFO	Select next point to add between
26 LP2_ADDPNT		TMS_MSG_INFO	Select position for new point 
27 AP_MOVPNT		TMS_MSG_INFO	Select point to move
28 LP_MOVPNT		TMS_MSG_INFO	Select new position for point
29 AP_DELPNT		TMS_MSG_INFO	Select point to delete
30 AP_LASSO		TMS_MSG_INFO 	Select start of lasso
31 LP_LASSO		TMS_MSG_INFO 	Select next point of lasso
32 LE_LASSO		TMS_MSG_ERROR   Lasso cannot be complex
33 AP_CONSTRUCT1        TMS_MSG_INFO    Select start of segment
34 AP_CONSTRUCT2        TMS_MSG_INFO    Select end of segment
35 LP_PARALLEL1         TMS_MSG_INFO    Select start of parallel
36 LP_PARALLEL2         TMS_MSG_INFO    Select end of parallel
37 LP_RAY               TMS_MSG_INFO    Select end of ray
38 LP_PERPENDICULAR     TMS_MSG_INFO    Select end of perpendicular
39 AP_BORDER		TMS_MSG_INFO 	Select edge to flip
40 AP_BOUND		TMS_MSG_INFO	Select bound origin
41 LE_TOO_SMALL	        TMS_MSG_ERROR	Cannot scale this small
42 AE_ORDER_NOT_DEEP	TMS_MSG_ERROR 	Cannot order deep objects
43 LP1_ARC_RADIUS	TMS_MSG_INFO	Enter radius of arc
44 LP2_ARC_DIRECTION	TMS_MSG_INFO	Select direction arc
45 AP_BRACE		TMS_MSG_INFO	Select start of brace
46 LP_BRACE		TMS_MSG_INFO	Select end of brace
47 AP_POLY_SELECT	TMS_MSG_INFO	Select 1st primitive
48 LP1_POLY_SELECT	TMS_MSG_INFO	Select 2nd primitive
49 AP_HBAR		TMS_MSG_INFO	Select centre of H-bar
50 AP_TBAR		TMS_MSG_INFO	Select which side of line for T-bar
51 LP_TBAR		TMS_MSG_INFO	Select point on line for base of T-bar
52 AP_ARC		TMS_MSG_INFO 	Select centre of arc
55 AE_INVAL_NAME	TMS_MSG_ERROR 	Invalid Name (Reserved word)
56 AE_OP_DOESNT_EXIST   TMS_MSG_ERROR   Specified operation does not exist
57 AE_GROUP_NOT_DEEP	TMS_MSG_ERROR 	Cannot group deep objects
58 AE_NO_OBJS_SELECTED  TMS_MSG_ERROR   No objects selected
59 LE_LASSO_NOPOLY	TMS_MSG_ERROR	No complete polygon within area
60 LE_LASSO_COMPLEX	TMS_MSG_ERROR	Complex polygon selected
61 AE_OPER_REQ_2_LINE   TMS_MSG_ERROR   Operation requires 2 polygons or lines to be selected
62 AE_JOIN_NO_COINC     TMS_MSG_ERROR   Cannot join, lines are not coincident
63 AE_SPLIT_REQ_1_LINE  TMS_MSG_ERROR	Split requires 1 line, 1 polygon, or 1 of each, to be selected
64 AP_SPLIT             TMS_MSG_INFO	Select point to split at
65 AE_SPLIT_INVALID_PNT TMS_MSG_ERROR   Invalid point selected for split
66 AP_POINT_IN_POLY	TMS_MSG_INFO    Select point to test
67 LP_POINT_INSIDE      TMS_MSG_INFO    Point selected was inside, select another point
68 LP_POINT_OUTSIDE     TMS_MSG_INFO    Point selected was outside, select another point
69 AE_HOLE_INVALID	TMS_MSG_INFO	Invalid objects selected for Hole operation
70 AP_UNHOLE		TMS_MSG_INFO	Select hole to remove
71 AE_NO_HOLES		TMS_MSG_ERROR   Operation requires a single holed polygon to be selected
72 LE_ADDPNT_SAME_POLY  TMS_MSG_ERROR   Add point requires two point from the same object
73 AE_NO_LINE_SELECTED  TMS_MSG_ERROR   Operation requires a single line to be selected
74 AE_OPER_REQ_1_POLY   TMS_MSG_ERROR   Operation requires a single polygon to be selected
75 LE_DELPNT_INVALID    TMS_MSG_ERROR   Deletion of selected point would create invalid object
76 LE_MOVPNT_INVALID    TMS_MSG_ERROR   Moving point to selected position would create invalid object
77 LE_ADDPNT_INVALID    TMS_MSG_ERROR   Adding point at selected position would create invalid object 
78 LE_CREATE_INVALID    TMS_MSG_ERROR   Cannot create invalid object (Eg. Complex polygon, empty text object)
79 AP_ROTATE		TMS_MSG_INFO    Select angle of rotation
80 AP_POLYGON		TMS_MSG_INFO 	Select start of polygon
81 LP_POLYGON		TMS_MSG_INFO 	Select next point of polygon
82 AP_CURVE_COPY	TMS_MSG_INFO    Select which side of line for parallel
83 TP_CURVE_DISTANCE    TMS_MSG_INFO    Enter distance from line for parallel
84 AE_NO_GAPS           TMS_MSG_ERROR   No gaps found in area
85 GAP_DLOG_YES		TMS_MSG_INFO    Yes
86 GAP_DLOG_NO          TMS_MSG_INFO    No
87 GAP_DLOG_CANCEL      TMS_MSG_INFO    Cancel
88 GAP_DLOG_PROMPT      TMS_MSG_INFO    Perform gap detection ?
89 AP_POLYGON_SELECT	TMS_MSG_INFO	Select start of selection polygon
90 AP_RECTANGLE		TMS_MSG_INFO 	Select first corner of rectangle
91 LP_RECTANGLE		TMS_MSG_INFO 	Select opposite corner of rectangle
92 AP_CIRCLE		TMS_MSG_INFO 	Select centre of circle
93 LP_CIRCLE		TMS_MSG_INFO 	Select point on circumference of circle
94 AP_POLYGON_SELECT TMS_MSG_INFO   Select next point of polygon or right locator to finish
95 AP_ELLIPSE		TMS_MSG_INFO 	Select centre of ellipse
96 LP1_ELLIPSE		TMS_MSG_INFO 	Select point on one axis of ellipse
97 LP2_ELLIPSE		TMS_MSG_INFO 	Select point on other axis of ellipse

# Now tablet reference messages
100 REF_PROMPT_1 TMS_MSG_INFO	Select 1st ref point in Window
101 REF_PROMPT_2 TMS_MSG_INFO	Select 2nd ref point in Window
102 REF_PROMPT_3 TMS_MSG_INFO	Select 1st ref point on Tablet
103 REF_PROMPT_4 TMS_MSG_INFO	Select 2nd ref point on Tablet
104 REF_PROMPT_5 TMS_MSG_INFO	Select 3rd ref point on Tablet
105 REF_PROMPT_6 TMS_MSG_INFO	Select 4th ref point on Tablet
106 CNTR_REF_PROMPT_1 TMS_MSG_INFO	Select 1st ref point on Tablet
107 CNTR_REF_PROMPT_2 TMS_MSG_INFO	Select 2nd ref point on Tablet

# Tablet validation enhancements
108 PERFORM_REF_PROMPT_1 TMS_MSG_INFO	Select point in Window
109 PERFORM_REF_PROMPT_2 TMS_MSG_INFO	Select point on Tablet

# Polygon split messages
110 AP_SPLIT_POLY   TMS_MSG_INFO	Select first end of split line
111 LP_SPLIT_POLY   TMS_MSG_INFO	Select opposite end of split line
112 LE_SPLIT_POLY   TMS_MSG_ERROR    Invalid split points selected
113 AE_SPLIT_POLY   TMS_MSG_ERROR   Split polygon cannot split holed polygons

# AUTO TITLE STUFF
200 AP_AUTO_TITLE  TMS_MSG_INFO Select first point for Auto Title polygon
201 LP1_AUTO_TITLE TMS_MSG_INFO Select next point for Auto Title polygon, Done when complete
202 LP2_AUTO_TITLE TMS_MSG_INFO Select all non-address point building seeds, then hit Done
203 AE_AUTO_TITLE TMS_MSG_ERROR No building seeds found, reselect extent

# Replace section messages
210 AP_REPLACESECTION		      TMS_MSG_INFO	Select start of section to replace
211 LP1_REPLACESECTION		      TMS_MSG_INFO	Select end of section to replace
212 LP2_REPLACESECTION		      TMS_MSG_INFO	Select an edge along section to replace 
213 LP3_REPLACESECTION		      TMS_MSG_INFO	Select position for new point 
214 LE_REPLACESECTION_SAME_POLY	      TMS_MSG_ERROR	Selected points must be on the same line or same ring of a polygon
215 LE_REPLACESECTION_DIFFERENT_POINT TMS_MSG_ERROR	Selected points must be different
216 LE_REPLACESECTION_INVALID_OBJECT  TMS_MSG_ERROR	Replacement of section with these points would create an invalid object (Eg. Complex polygon).  Use backup to step back and create a valid section.
217 LP4_REPLACESECTION		      TMS_MSG_INFO	Select new point for junction

# Extrusion messages
220 AP_EXTRUSION		      TMS_MSG_INFO	Select start of section to extrude
221 LP1_EXTRUSION		      TMS_MSG_INFO	Select end of section to extrude
222 LP2_EXTRUSION		      TMS_MSG_INFO	Select an edge along section to extrude, and extrusion side
223 LP3_EXTRUSION		      TMS_MSG_INFO	Select position for extruded section 
224 LP4_EXTRUSION		      TMS_MSG_INFO	Select which side of line for extrusion
225 TP_EXTRUSION		      TMS_MSG_INFO      Enter distance from original section for extrusion
226 LE_EXTRUSION_SAME_POLY	      TMS_MSG_ERROR	Selected points must be on the same object 
227 LE_EXTRUSION_DIFFERENT_POINT      TMS_MSG_ERROR	Selected points must be different
228 LE_EXTRUSION_INVALID_OBJECT	      TMS_MSG_ERROR	Extrusion of section at this position would create an invalid object (Eg. Complex or multiple polygons).  Use backup to step back and choose a valid position or section.

# Win select messages
230 AP_WINSELECT			TMS_MSG_INFO	Select object or operation

# Win rotate messages
240 AE_INVALID_ROTATE_ACTIVATE_DATA   TMS_MSG_ERROR Invalid endpoint passed to winrotate activate

# APP 6A
300 AP_APP6A_PALETTE  TMS_MSG_INFO  Select position for APP6A symbol
301 AE_APP6A_PALETTE  TMS_MSG_ERROR Error getting symbol information
302 AE_APP6A_NOTHING_SELECTED TMS_MSG_ERROR No symbol selected in palette