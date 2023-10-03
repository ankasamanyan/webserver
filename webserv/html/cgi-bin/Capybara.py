import os
# import time
# time.sleep(10) 
print("Hello, world!")

output_file_path = os.environ.get("OUTPUT_FILE")

if output_file_path:
        with open(output_file_path, "w") as output_file:
            
            query_string = os.environ.get("QUERY_STRING")

            if query_string:
                    output_file.write('<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0"><title>✨42Capybaras✨</title><style>body{background-color:white;font-family:Arial,sans-serif;margin:0;padding:0;display:flex;flex-direction:column;justify-content:center;align-items:center;min-height:100vh;}.topbar{background-color:white;color:#6c55be;padding:30px 0;font-size:23px;display:flex;justify-content:space-between;align-items:center;position:fixed;top:0;width:100%;z-index:999;}.topbar a{color:#6c55be;text-decoration:none;margin:0 50px;}.company-name{color:#6c55be;margin:0 50px;}.content{display:flex;flex-direction:row;justify-content:space-between;align-items:center;padding:100px;}.text{text-align:center;flex:50%;}h1{font-size:48px;color:#333333;}p{font-size:24px;color:#666;}.image{flex:50%;}img{max-width:100%;height:auto;}.button{display:inline-block;padding:20px 30px;background-color:transparent;color:#6c55be;text-decoration:none;font-weight:bold;border:2px solid #6c55be;border-radius:10px;margin-top:20px;transition:background-color 0.3s ease,color 0.3s ease;}.button:hover{background-color:#6c55be;color:white;animation:backgroundColorChange 5s linear infinite;border:2px solid transparent;}.button:focus{outline:none;}</style></head><body><div class="topbar"><div class="company-name"><a href="/">✨42Capybaras✨</a></div><div class="links"><a href="../Uploads.html">Upload</a><a href="../cgi-form.html">CGI</a><a href="../Contact-us.html">Contact us</a></div></div><div class="content"><div class="text"><h1>Your Capybara name is ')
                    output_file.write(query_string + 'bara')
                    output_file.write('</h1><p>Welcome to the Capybara world! Feel like home!</p><a class="button" href="/">Back to Home</a></div><div class="image"><img src="../img/20.JPG" alt="Image"></div></div></body></html>')
            else:
                	output_file.write('<!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1.0"><title>✨42Capybaras✨</title><style>body{background-color:white;font-family:Arial,sans-serif;margin:0;padding:0;display:flex;flex-direction:column;justify-content:center;align-items:center;min-height:100vh;}.topbar{background-color:white;color:#6c55be;padding:30px 0;font-size:23px;display:flex;justify-content:space-between;align-items:center;position:fixed;top:0;width:100%;z-index:999;}.topbar a{color:#6c55be;text-decoration:none;margin:0 50px;}.company-name{color:#6c55be;margin:0 50px;}.content{display:flex;flex-direction:row;justify-content:space-between;align-items:center;padding:100px;}.text{text-align:center;flex:50%;}h1{font-size:48px;color:#333333;}p{font-size:24px;color:#666;}.image{flex:50%;}img{max-width:100%;height:auto;}.button{display:inline-block;padding:20px 30px;background-color:transparent;color:#6c55be;text-decoration:none;font-weight:bold;border:2px solid #6c55be;border-radius:10px;margin-top:20px;transition:background-color 0.3s ease,color 0.3s ease;}.button:hover{background-color:#6c55be;color:white;animation:backgroundColorChange 5s linear infinite;border:2px solid transparent;}.button:focus{outline:none;}</style></head><body><div class="topbar"><div class="company-name"><a href="/">✨42Capybaras✨</a></div><div class="links"><a href="../Uploads.html">Upload</a><a href="../cgi-form.html">CGI</a><a href="../Contact-us.html">Contact us</a></div></div><div class="content"><div class="text"><h1>Your Capybara name is regular Capybara</h1><p>Welcome to the Capybara world! Feel like home!</p><a class="button" href="/">Back to Home</a></div><div class="image"><img src="../img/20.JPG" alt="Image"></div></div></body></html>')

else:
    print("The OUTPUT_FILE environment variable is not set.")
