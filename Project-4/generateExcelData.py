import pprint
import xlsxwriter

categories = ['Precipitation','Temperature','Grain Height','NumDeer','Locust']

data = {}

##############
# Parse Data
##############
# valsForExcel.txt = <Month>,<Year>,<Precipitation>,<Temperature>,<Grain Height>,<NumDeer>,<Locust>
# data = {
# 			"Precipitation": [],
# 			"Temperature": [],
#			...
# 		}
aggregate = {"Precipitation": [], 'Temperature': [],'Height': [],'NumDeer': [],'Locust': []}
with open('valsForExcel.txt') as fp:
	for line in fp:
		parsed = [float(x.strip()) for x in line.split(',')]

		month = parsed[0]
		precip = parsed[1]
		temp = parsed[2]
		height = parsed[3]
		numDeer = parsed[4]
		locust = parsed[5]

		aggregate["Precipitation"].append(precip)
		aggregate["Temperature"].append(temp)
		aggregate["Height"].append(height)
		aggregate["NumDeer"].append(numDeer)
		aggregate["Locust"].append(locust)


pp = pprint.PrettyPrinter(indent=4)
pp.pprint(aggregate)

workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()

col = 1
for monthNum in range(0, len(aggregate["Height"])):
	worksheet.write(0, col, monthNum)
	col += 1

row = 1
for cat in categories:
	worksheet.write(row, 0, cat)
	row += 1

row = 1
for cat, arr in aggregate.items():
	col = 1
	for value in arr:
		worksheet.write(row, col, value)
		col += 1
	row += 1

##############################################
#			LINE GRAPH Speedup v. NumPads
# 	Values = Y axis	   Categories = X axis
##############################################
chart1 = workbook.add_chart({'type': 'line'})

lastCol = len(aggregate["Height"]) + 1
for catNum in range(1, len(categories) + 1):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       categories[catNum - 1],
        'values': ['Sheet1', catNum, 1, catNum, lastCol],
        'categories':     ['Sheet1', 0, 1, 0, lastCol],
    })


# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Month',
})
chart1.set_y_axis({
	'name': 'Quantity',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G15', chart1, {'x_scale': 2, 'y_scale': 2})
