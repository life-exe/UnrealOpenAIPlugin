import json
import sys

def update_json_file(file_path):
    # Load the JSON from the file
    with open(file_path, 'r') as file:
        data = json.load(file)

    # Update key for Blueprint Packaging
    data["EnabledByDefault"] = False

    # Write the updated JSON back to the file
    with open(file_path, 'w') as file:
        json.dump(data, file, indent=4)  # `indent=4` for pretty-printing

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: script_name.py <path_to_json_file>")
        sys.exit(1)
    file_path = sys.argv[1]
    update_json_file(file_path)