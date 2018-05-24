import pprint
import xlsxwriter

NumberOfTrialsPerThread = 8

arrayMultData = {}
MultnumElementsValues = []
MultlocalSizeValues = []

arrayMultAddData = {}
MultAddnumElementsValues = []
MultAddlocalSizeValues = []

arrayMultReductionData = {}
MultRednumElementsValues = []
MultRedlocalSizeValues = []
##############
# Parse Data
##############

# valsForExcel.txt = <LOCAL_SIZE>,<NUM_ELEMENTS>,<GigaMults>
# data = {
# 	<LOCAL_SIZE>:{
# 		<NUM_ELEMENTS>: <GigaMults>
# 	}
# }
with open('arrayMult.txt') as fp:
	for line in fp:
		parsed = [float(x.strip()) for x in line.split(',')]
		localSize = parsed[0]
		numElements = parsed[1]
		GigaMults = parsed[2]
		if numElements not in MultnumElementsValues:
			MultnumElementsValues.append(numElements)
		if localSize not in MultlocalSizeValues:
			MultlocalSizeValues.append(localSize)

		if localSize not in arrayMultData:
			arrayMultData[localSize] = {}
		arrayMultData[localSize][numElements] = GigaMults

with open('arrayMultAdd.txt') as fp2:
	for line in fp2:
		parsed = [float(x.strip()) for x in line.split(',')]
		localSize = parsed[0]
		numElements = parsed[1]
		GigaMults = parsed[2]
		if numElements not in MultAddnumElementsValues:
			MultAddnumElementsValues.append(numElements)
		if localSize not in MultAddlocalSizeValues:
			MultAddlocalSizeValues.append(localSize)

		if localSize not in arrayMultAddData:
			arrayMultAddData[localSize] = {}
		arrayMultAddData[localSize][numElements] = GigaMults

with open('arrayMultReduction.txt') as fp3:
	for line in fp3:
		parsed = [float(x.strip()) for x in line.split(',')]
		localSize = parsed[0]
		numElements = parsed[1]
		GigaMults = parsed[2]
		if numElements not in MultRednumElementsValues:
			MultRednumElementsValues.append(numElements)
		if localSize not in MultRedlocalSizeValues:
			MultRedlocalSizeValues.append(localSize)

		if localSize not in arrayMultReductionData:
			arrayMultReductionData[localSize] = {}
		arrayMultReductionData[localSize][numElements] = GigaMults

pp = pprint.PrettyPrinter(indent=4)
pp.pprint(arrayMultData)
pp.pprint(arrayMultAddData)
pp.pprint(arrayMultReductionData)



workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()
bold_cell_format = workbook.add_format()
bold_cell_format.set_bold()
############# ArrayMult writing table to excel #############
col = 1
for numEle in MultnumElementsValues:
	worksheet.write(0,col, numEle, bold_cell_format)
	col += 1

row = 1
for localSize, values in arrayMultData.items():
	col = 1
	worksheet.write(row, 0, localSize, bold_cell_format)
	for numEle, Gigas in values.items():
		worksheet.write(row, col, Gigas)
		col += 1
	row += 1

############# ArrayMultAdd writing table to excel #############
row += 2
col = 1
for numEle in MultAddnumElementsValues:
	worksheet.write(row,col, numEle, bold_cell_format)
	col += 1

row += 1
startMultAdd = row
for localSize, values in arrayMultAddData.items():
	col = 1
	worksheet.write(row, 0, localSize, bold_cell_format)
	for numEle, Gigas in values.items():
		worksheet.write(row, col, Gigas)
		col += 1
	row += 1

############# ArrayMultRed writing table to excel #############
row += 2
col = 1
for numEle in MultRednumElementsValues:
	worksheet.write(row,col, numEle, bold_cell_format)
	col += 1

row += 1
startMultRed = row
for localSize, values in arrayMultReductionData.items():
	col = 1
	worksheet.write(row, 0, localSize, bold_cell_format)
	for numEle, Gigas in values.items():
		worksheet.write(row, col, Gigas)
		col += 1
	row += 1



############# ArrayMult Graph building #############
##############################################
#			LINE GRAPH GigaMults v. Global Work Size
# 	Values = Y axis	   Categories = X axis
#	Values = GigaMults Categories = Num Elements (global work size)
##############################################

chart1 = workbook.add_chart({'type': 'line'})
rowValues = 1
for i in MultlocalSizeValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, len(MultnumElementsValues)],
        'categories':     ['Sheet1', 0, 1, 0, len(MultnumElementsValues)],
    })
	rowValues += 1

# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Global Work Size (size of arrays)',
})
chart1.set_y_axis({
	'name': 'GigaMultiplies Per Second',
})

currentInsertDepth = 1
insert = str(chr(65 + len(MultnumElementsValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart1)

##############################################
#			LINE GRAPH GigaMults v. Local Work Size
# 	Values = Y axis	   Categories = X axis
#	Values = GigaMults Categories = Local Work Size
##############################################
chart2 = workbook.add_chart({'type': 'line'})
rowValues = 1
for i in range(0, len(MultnumElementsValues)):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart2.add_series({
        'name':       str(MultnumElementsValues[i]),
        'values': ['Sheet1', 1, i + 1, len(MultlocalSizeValues), i + 1],
        'categories':     ['Sheet1', 1, 0, len(MultlocalSizeValues), 0],
    })
	rowValues += 1

# Configure the chart axes.
chart2.set_x_axis({
	'name': 'Local Work Size',
})
chart2.set_y_axis({
	'name': 'GigaMultiplies Per Second',
})

currentInsertDepth += 18
insert = str(chr(65 + len(MultnumElementsValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart2)



############# ArrayMultAdd Graph building #############
##############################################
#			LINE GRAPH GigaMults v. Global Work Size
# 	Values = Y axis	   Categories = X axis
#	Values = GigaMults Categories = Num Elements (global work size)
##############################################
chart3 = workbook.add_chart({'type': 'line'})
rowValues = startMultAdd
for i in MultAddlocalSizeValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart3.add_series({
        'name':       str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, len(MultAddnumElementsValues)],
        'categories':     ['Sheet1', 0, 1, 0, len(MultAddnumElementsValues)],
    })
	rowValues += 1

# Configure the chart axes.
chart3.set_x_axis({
	'name': 'Global Work Size (size of arrays)',
})
chart3.set_y_axis({
	'name': 'GigaMultiplies And Adds Per Second',
})
currentInsertDepth += 18
insert = str(chr(65 + len(MultAddnumElementsValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart3)

##############################################
#			LINE GRAPH GigaMults v. Local Work Size
# 	Values = Y axis	   Categories = X axis
#	Values = GigaMults Categories = Local Work Size
##############################################
chart4 = workbook.add_chart({'type': 'line'})
rowValues = startMultAdd
for i in range(0, len(MultAddnumElementsValues)):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart4.add_series({
        'name':       "Size Array: " + str(MultAddnumElementsValues[i]),
        'values': ['Sheet1', rowValues, i + 1, rowValues + len(MultAddlocalSizeValues), i + 1],
        'categories':     ['Sheet1', 1, 0, len(MultAddlocalSizeValues), 0],
    })
	# rowValues += 1

# Configure the chart axes.
chart4.set_x_axis({
	'name': 'Local Work Size',
})
chart4.set_y_axis({
	'name': 'GigaMultiplies And Adds Per Second',
})
currentInsertDepth += 18
insert = str(chr(65 + len(MultAddnumElementsValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart4)



############# ArrayMultRed Graph building #############
##############################################
#			LINE GRAPH GigaMults v. Global Work Size
# 	Values = Y axis	   Categories = X axis
#	Values = GigaMults Categories = Num Elements (global work size)
##############################################
chart5 = workbook.add_chart({'type': 'line'})
rowValues = startMultRed
for i in MultRedlocalSizeValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart5.add_series({
        'name':       str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, len(MultRednumElementsValues)],
        'categories':     ['Sheet1', 0, 1, 0, len(MultRednumElementsValues)],
    })
	rowValues += 1

# Configure the chart axes.
chart5.set_x_axis({
	'name': 'Global Work Size (size of arrays)',
})
chart5.set_y_axis({
	'name': 'GigaMultiplies Reductions Per Second',
})
currentInsertDepth += 18
insert = str(chr(65 + len(MultRednumElementsValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart5)
