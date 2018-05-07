import pprint
import xlsxwriter

NumberOfTrialsPerThread = 8

data = {}
data2 = {}


##############
# Parse Data
##############
# valsForExcel.txt = <threads>,<numPads>,<time>
# data = {
# 	<numThread>:{
# 		<numPad>: <time>
# 	}
# }
with open('valsForExcel.txt') as fp:
	for line in fp:
		parsed = [int(x.strip()) for x in line.split(',')]
		numThreads = parsed[0]
		numPads = parsed[1]
		time = parsed[2]
		if numPads == 16:
			continue
		if numThreads not in data:
			data[numThreads] = {}
		data[numThreads][numPads] = time

with open('fix2.txt') as fp2:
	for line in fp2:
		parsed = [int(x.strip()) for x in line.split(',')]
		numThreads = parsed[0]
		time = parsed[1]

		if numThreads not in data2:
			data2[numThreads] = {}
		data2[numThreads] = time

pp = pprint.PrettyPrinter(indent=4)
pp.pprint(data)
pp.pprint(data2)

workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()

# FIX 1: Write headers
worksheet.write(0,0, "Num Threads")
col = 1
for header in range(0,16):
	worksheet.write(0,col, header)
	col += 1

row = 1
for threadNum, values in data.items():
	col = 1
	worksheet.write(row, 0, threadNum)
	for pad, time in values.items():
		T1 = data[1][pad]
		TN = data[threadNum][pad]
		worksheet.write(row, col, (T1/TN))
		col += 1
	row += 1

# FIX 2: Write headers
row += 2
worksheet.write(row,0, "Num Threads")
worksheet.write(row,1, "Speedup")
row += 1
startFix2 = row
for threadNum, time in data2.items():
	col = 1
	worksheet.write(row, 0, threadNum)
	T1 = data2[1]
	TN = data2[threadNum]
	for i in range(0,16):
		worksheet.write(row, col, (T1/TN))
		col += 1
	row += 1

##############################################
#			LINE GRAPH Speedup v. NumPads
# 	Values = Y axis	   Categories = X axis
#	Values = Speedup Categories = NumPads
##############################################
chart1 = workbook.add_chart({'type': 'line'})
endRow = row
startRow = 1
thread = 1;
rowValues = 1
# FIX 1 
for i in (1,2,4,6):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       "Fix1: Num Threads " + str(i),
        'values': ['Sheet1', rowValues, 1,   rowValues, 17],
        'categories':     ['Sheet1', 0, 1, 0, 17],
    })
	rowValues += 1
# FIX 2
for i in (1,2,4,6):
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
        'name':       "Fix2: Num Threads " + str(i),
        'values': ['Sheet1', startFix2, 1,  startFix2, 17],
        'categories':     ['Sheet1', 0, 1, 0, 17],
    })
	startFix2 += 1

# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Number of Pads',
})
chart1.set_y_axis({
	'name': 'Speedup',
})

# Insert the chart into the worksheet.
worksheet.insert_chart('G15', chart1)
