import pprint
import xlsxwriter

NumberOfTrialsPerThread = 8

# Create some sample data to plot.
maxRow = 0
categories  = ['Num Nodes', '1 Thread', '2 Threads', '4 Threads', '6 Threads']
data = {}


##############
# Parse Data
##############
# <threads>,<nodes>,<megaFunc>
with open('valsForExcel.txt') as fp:
	for line in fp:
		maxRow += 1
		parsed = [float(x.strip()) for x in line.split(',')]
		if parsed[1] not in data:
			data[parsed[1]] = dict()
		if parsed[0] not in data[parsed[1]]:
			data[parsed[1]][parsed[0]] = dict()
		data[parsed[1]][parsed[0]] = parsed[2]

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
for nodeNum, values in data.items():
	col = 1
	worksheet.write(row, 0, nodeNum)
	for threadNum, megaFunc in values.items():
		worksheet.write(row, col, megaFunc)
		col += 1
	row += 1
		

##############################################
#			LINE GRAPH MegaFun v. NumNodes
# 	Values = Y axis	   Categories = X axis
#	Values = MegaFuncs Categories = NumNodes
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
	'name': 'Number of Nodes',
})
chart1.set_y_axis({
	'name': 'Mega Heights Evaluations Per Second',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G2', chart1)



##############################################
#			LINE GRAPH MegaFun v. NumThreads
# 	Values = Y axis	   Categories = X axis
#	Values = MegaFuncs Categories = NumNodes
##############################################
chart2 = workbook.add_chart({'type': 'line'})
startCol = 1
endCol = 4
row = 1
for nodeNum in data:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart2.add_series({
        'name':       str(nodeNum),
        'values': ['Sheet1', row, startCol , row, endCol],
		'categories': '=Sheet1!$B$1:$E$1',
    })
	row += 1

# Configure the chart axes.
chart2.set_x_axis({
	'name': 'Number of Threds',
})
chart2.set_y_axis({
	'name': 'Mega Heights Evaluations Per Second',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G16', chart2)

workbook.close()