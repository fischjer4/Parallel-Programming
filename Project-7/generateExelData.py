import pprint
import xlsxwriter


totalParticles = {}
particlesValues = []
megaPartsValues = []

maxRow = 0
##############
# Parse Data
##############
with open('particlePerformance.txt') as fp:
	for line in fp:
		parsed = [float(x.strip()) for x in line.split('\t')]
		numParticles = parsed[0]
		megaPartsPerSec = parsed[1]

		if numParticles not in particlesValues:
			particlesValues.append(numParticles)
		if megaPartsPerSec not in megaPartsValues:
			megaPartsValues.append(megaPartsPerSec)

		if numParticles not in totalParticles:
			totalParticles[numParticles] = []
		totalParticles[numParticles].append(megaPartsPerSec)

pp = pprint.PrettyPrinter(indent=4)
pp.pprint(totalParticles)


workbook = xlsxwriter.Workbook('results.xlsx')
worksheet = workbook.add_worksheet()
bold_cell_format = workbook.add_format()
bold_cell_format.set_bold()
############# writing table to excel #############
col = 1
avg = 0
print("------ Averages -----")
for numParts, values in totalParticles.items():
	worksheet.write(0, col, numParts, bold_cell_format)
	row = 1
	for megaPartPer in values:
		avg += megaPartPer
		worksheet.write(row, col, megaPartPer)
		row += 1
	print(str(numParts) + ": \t" + str( int(avg/ len(values)) ) )
	col += 1
	if maxRow < row:
		maxRow = row

############# Graph building #############
##############################################
#  LINE GRAPH MegaParticlesPerSec v. Total Num Particles
# 	Values = Y axis	   Categories = X axis
#	Values = MegaParticles Categories = Num Particles
##############################################

chart1 = workbook.add_chart({'type': 'bar'})
col = 1
startRow = 1
lastRow = maxRow
for i in particlesValues:
	# [sheetname, first_row, first_col, last_row, last_col]
	chart1.add_series({
		'name': "Particles: " + str(i),
        'values': ['Sheet1', startRow, col,   lastRow, col],
        'categories': ['Sheet1', 0, col, 0, col]
    })
	col += 1


# Configure the chart axes.
chart1.set_x_axis({
	'name': 'Total Number of Particles',
})
chart1.set_y_axis({
	'name': 'MegaParticles Per Second',
})

currentInsertDepth = 1
insert = str(chr(65 + len(particlesValues) + 1)) + str(currentInsertDepth)
# Insert the chart into the worksheet.
worksheet.insert_chart(insert, chart1)
