import os
# import time
# time.sleep(10) 
print("Hello, world!")

output_file_path = os.environ.get("OUTPUT_FILE")

if output_file_path:
    # try:
        with open(output_file_path, "w") as output_file:
            
            # query_string = os.environ.get("QUERY_STRING")

            # Perform computations based on the value of QUERY_STRING
            # if query_string:
                    output_file.write("Halp")
                # Example: If QUERY_STRING contains "option1=true", write "Option 1 is selected" to the file
                # if "option1=true" in query_string:
                # Add more conditions based on your requirements
    #     print(f"Output written to {output_file_path}")
    # except IOError as e:
        # print(f"Error opening/writing to {output_file_path}: {e}")
else:
    print("The OUTPUT_FILE environment variable is not set.")
