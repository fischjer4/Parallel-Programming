import pprint
import xlsxwriter

NumberOfTrialsPerThread = 8

# Create some sample data to plot.
maxRow = 0
categories  = ['Num Threads', 'coarse+static', 'coarse+dynamic', 'fine+static', 'fine+dynamic']
data = {}


##############
# Parse Data
##############
# <threads>,<megaFuncCS>,<megaFuncCD>,<megaFuncFS>,<megaFuncFD>
with open('valsForExcel.txt') as fp:
	for line in fp:
		maxRow += 1
		parsed = [float(x.strip()) for x in line.split(',')]
		if parsed[0] not in data:
			data[parsed[0]] = []
		data[parsed[0]].append(parsed[1])
		data[parsed[0]].append(parsed[2])
		data[parsed[0]].append(parsed[3])
		data[parsed[0]].append(parsed[4])

pp = pprint.PrettyPrinter(indent=4)
pp.pprint(data)

workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()

# Write headers
col = 0
for header in categories:
	worksheet.write(0,col, header)
	col += 1

row = 1
for threadNum, values in data.items():
	col = 1
	worksheet.write(row, 0, threadNum)
	for megaFunc in values:
		worksheet.write(row, col, megaFunc)
		col += 1
	row += 1
		

##############################################
#			LINE GRAPH MegaFun v. NumThreads
# 	Values = Y axis	   Categories = X axis
#	Values = MegaFuncs Categories = NumThreads
##############################################
chart1 = workbook.add_chart({'type': 'line'})
endRow = row
startRow = 1
for i in range(len(categories) - 1):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       categories[i + 1],
        'values': ['Sheet1', startRow, i + 1,   endRow, i + 1],
        'categories':     ['Sheet1', startRow, 0, endRow, 0],
    })

# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Number of Threads',
})
chart1.set_y_axis({
	'name': 'Mega Bodies Compared Per Second',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G2', chart1)



##############################################
#			LINE GRAPH MegaFun v. NumThreads
# 	Values = Y axis	   Categories = X axis
#	Values = MegaFuncs Categories = Num Threads
##############################################
# chart2 = workbook.add_chart({'type': 'line'})
# startCol = 1
# endCol = 4
# row = 1
# for nodeNum in data:
# 	# [sheetname, first_row, first_col, last_row, last_col]
# 	chart2.add_series({
#         'name':       str(nodeNum),
#         'values': ['Sheet1', row, startCol , row, endCol],
# 		'categories': '=Sheet1!$B$1:$E$1',
#     })
# 	row += 1

# # Configure the chart axes.
# chart2.set_x_axis({
# 	'name': 'Number of Threads',
# })
# chart2.set_y_axis({
# 	'name': 'Mega Bodies Compared Per Second',
# })

# # Insert the chart into the worksheet.
# worksheet.insert_chart('G16', chart2)

# workbook.close()