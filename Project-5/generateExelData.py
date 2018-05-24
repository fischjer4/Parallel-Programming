import pprint
import xlsxwriter

NumberOfTrialsPerThread = 8

noSIMDdata = {}
yesSIMDdata = {}
numThreadValues = []
numsValues = []

##############
# Parse Data
##############
# valsForExcel.txt = <threads>,<numPads>,<time>
# data = {
# 	<numThread>:{
# 		<numPad>: <time>
# 	}
# }
with open('noSIMDvalues.txt') as fp:
	for line in fp:
		parsed = [int(x.strip()) for x in line.split(',')]
		numThreads = parsed[0]
		nums = parsed[1]
		time = parsed[2]
		if numThreads not in numThreadValues:
			numThreadValues.append(numThreads)
		if nums not in numsValues:
			numsValues.append(nums)

		if numThreads not in noSIMDdata:
			noSIMDdata[numThreads] = {}
		noSIMDdata[numThreads][nums] = time

with open('yesSIMDdata.txt') as fp2:
	for line in fp2:
		parsed = [int(x.strip()) for x in line.split(',')]
		numThreads = parsed[0]
		nums = parsed[1]
		time = parsed[2]
		if numThreads not in yesSIMDdata:
			yesSIMDdata[numThreads] = {}
		yesSIMDdata[numThreads][nums] = time

pp = pprint.PrettyPrinter(indent=4)
pp.pprint(noSIMDdata)
pp.pprint(yesSIMDdata)

workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()

############# NO SIMD writing table to excel #############
worksheet.write(0,0, "Num Threads")
col = 1
for num in numsValues:
	worksheet.write(0,col, num)
	col += 1

row = 1
for threadNum, values in noSIMDdata.items():
	col = 1
	worksheet.write(row, 0, threadNum)
	for num, time in values.items():
		T1 = noSIMDdata[1][num]
		TN = noSIMDdata[threadNum][num]
		worksheet.write(row, col, (T1/TN))
		col += 1
	row += 1

############# YES SIMD writing table to excel #############
row += 2
worksheet.write(row,0, "Num Threads")
col = 1
for num in numsValues:
	worksheet.write(0,col, num)
	col += 1

row += 1
startYesSIMD = row
for threadNum, values in yesSIMDdata.items():
	col = 1
	worksheet.write(row, 0, threadNum)
	for num, time in values.items():
		T1 = yesSIMDdata[1][num]
		TN = yesSIMDdata[threadNum][num]
		worksheet.write(row, col, (T1/TN))
		col += 1
	row += 1




############# NO SIMD Graph building #############
##############################################
#			LINE GRAPH MegaMults v. Nums
# 	Values = Y axis	   Categories = X axis
#	Values = MegaMults Categories = Nums
##############################################

chart1 = workbook.add_chart({'type': 'line'})
rowValues = 1
for i in numThreadValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       "NoSIMD: Num Threads " + str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, len(numsValues)],
        'categories':     ['Sheet1', 0, 1, 0, len(numsValues)],
    })
	rowValues += 1

# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Size Of Arrays',
})
chart1.set_y_axis({
	'name': 'Speedup',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G15', chart1)

##############################################
#			LINE GRAPH MegaMults v. Nums
# 	Values = Y axis	   Categories = X axis
#	Values = MegaMults Categories = Num Threads
##############################################
chart2 = workbook.add_chart({'type': 'line'})
rowValues = 1
for i in range(0, len(numsValues)):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart2.add_series({
        'name':       "Size Array: " + str(numsValues[i]),
        'values': ['Sheet1', 1, i + 1, len(numThreadValues), i + 1],
        'categories':     ['Sheet1', 1, 0, len(numThreadValues), 0],
    })
	rowValues += 1

# Configure the chart axes.
chart2.set_x_axis({
	'name': 'Number of Threads (No SIMD)',
})
chart2.set_y_axis({
	'name': 'Speedup',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G35', chart2)



############# YES SIMD Graph building #############
chart3 = workbook.add_chart({'type': 'line'})
rowValues = startYesSIMD
for i in numThreadValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart3.add_series({
        'name':       "YesSIMD: Num Threads " + str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, len(numsValues)],
        'categories':     ['Sheet1', 0, 1, 0, len(numsValues)],
    })
	rowValues += 1

# Configure the chart axes.
chart3.set_x_axis({
	'name': 'Size Of Arrays',
})
chart3.set_y_axis({
	'name': 'Speedup',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G55', chart3)

##############################################
#			LINE GRAPH MegaMults v. Nums
# 	Values = Y axis	   Categories = X axis
#	Values = MegaMults Categories = Num Threads
##############################################
chart4 = workbook.add_chart({'type': 'line'})
rowValues = startYesSIMD
for i in range(0, len(numsValues)):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart4.add_series({
        'name':       "Size Array: " + str(numsValues[i]),
        'values': ['Sheet1', rowValues, i + 1, rowValues + len(numThreadValues), i + 1],
        'categories':     ['Sheet1', 1, 0, len(numThreadValues), 0],
    })
	# rowValues += 1

# Configure the chart axes.
chart4.set_x_axis({
	'name': 'Number of Threads (Yes SIMD)',
})
chart4.set_y_axis({
	'name': 'Speedup',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G75', chart4)
