import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Usage: python repair.py <input_file.ipynb>")
    sys.exit(1)

# Get the input file path from the command-line argument
input_file_path = sys.argv[1]

# Define the output file path (same as input file for simplicity)
output_file_path = input_file_path

# Define the character combinations and their replacements
character_combinations = {
    (196, 353, 313, 351): "ż", #OK
    (196, 353, 313, 350): "Ż",
    (196, 353, 313, 159): "ź", #OK
    (196, 353, 313, 158): "Ź",
    (196, 130, 313, 130): "ó", #OK
    (196, 130, 194, 147): "Ó", #OK
    (196, 353, 194, 130): "ł", #OK
    (196, 353, 194, 129): "Ł", #OK
    (196, 353, 194, 155): "ś", #OK
    (196, 353, 194, 154): "Ś", #OK
    (196, 353, 194, 132): "ń", #OK
    (196, 353, 194, 131): "Ń",
    (258, 132, 194, 135): "ć", #OK
    (258, 132, 194, 134): "Ć", #OK
    (258, 132, 194, 133): "ą", #OK
    (258, 132, 194, 132): "Ą",
    (258, 132, 194, 153): "ę", #OK
    (258, 132, 194, 152): "Ę" #OK
}

try:
    # Read the content of the input file
    with open(input_file_path, 'r', encoding='ISO-8859-2') as input_file:
        content = input_file.read()

    # Replace characters based on their numerical representations
    fixed_content = content
    for char_codes, replacement in character_combinations.items():
        char_to_replace = ''.join(chr(code) for code in char_codes)
        fixed_content = fixed_content.replace(char_to_replace, replacement)

    # Write the fixed content to the output file
    with open(output_file_path, 'w', encoding='UTF-8') as output_file:
        output_file.write(fixed_content)

    print("File repaired successfully!")

except FileNotFoundError:
    print(f"File '{input_file_path}' not found.")
    sys.exit(1)

except Exception as e:
    print(f"An error occurred: {e}")
    sys.exit(1)
