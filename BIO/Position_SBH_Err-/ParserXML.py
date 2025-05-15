import xml.etree.ElementTree as ET

# Load the XML file
tree = ET.parse('sample.xml')
root = tree.getroot()

# Open a text file to write the extracted data
with open('TestCase.txt', 'w') as f:
    # Initialize the count of probes
    num_probes = 0
    # Iterate through the XML tree and write nucleotide, posL, and posH for each probe
    for cell in root.findall('.//cell'):
        posL = int(cell.attrib.get('posL'))  # Convert posL to int for comparison
        if posL == 1 and cell.text == root.attrib.get('start'):
            continue  # Skip writing this line to output.txt
        else:
            num_probes += 1  # Increment the count of probes
            posH = cell.attrib.get('posH')
            cell_value = cell.text
            f.write(cell_value + ' ' + str(posL) + ' ' + posH + '\n')

# Reopen the file in append mode to add content at the beginning
with open('TestCase.txt', 'r+') as f:
    content = f.read()
    f.seek(0, 0)  # Move cursor to the beginning of the file
    f.write(root.attrib.get('length') + '\n')
    f.write(root.attrib.get('start') + '\n')  # Write the start sequence
    f.write(str(num_probes) + '\n')  # Write the number of probes
    f.write(content)  # Write back the original content

print("Data has been written to 'TestCase.txt' successfully.")
