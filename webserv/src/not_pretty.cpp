#include "../includes/Server_handler.hpp"

void	Client::checkHeaders(std::stringstream &headers)
{
	std::string		ext;
	size_t 			ext_in = _path.rfind('.');

	if (ext_in != std::string::npos)
		ext = _path.substr(ext_in);
	if (ext.compare(".html") == 0)
		headers << "content-type: text/html\r\n";
	else if (ext.compare(".htm") == 0)
		headers << "content-type: text/html\r\n";
	else if (ext.compare(".css") == 0)
		headers << "content-type: text/css\r\n";
	else if (ext.compare(".js") == 0)
		headers << "content-type: application/javascript\r\n";
	else if (ext.compare(".json") == 0)
		headers << "content-type: application/json\r\n";
	else if (ext.compare(".jpg") == 0)
		headers << "content-type: image/jpeg\r\n";
	else if (ext.compare(".jpeg") == 0)
		headers << "content-type: image/jpeg\r\n";
	else if (ext.compare(".png") == 0)
		headers << "content-type: image/png\r\n";
	else if (ext.compare(".gif") == 0)
		headers << "content-type: image/gif\r\n";
	else if (ext.compare(".bmp") == 0)
		headers << "content-type: image/bmp\r\n";
	else if (ext.compare(".ico") == 0)
		headers << "content-type: image/x-icon\r\n";
	else if (ext.compare(".svg") == 0)
		headers << "content-type: image/svg+xml\r\n";
	else if (ext.compare(".xml") == 0)
		headers << "content-type: application/xml\r\n";
	else if (ext.compare(".pdf") == 0)
		headers << "content-type: application/pdf\r\n";
	else if (ext.compare(".zip") == 0)
		headers << "content-type: application/zip\r\n";
	else if (ext.compare(".gz") == 0)
		headers << "content-type: application/gzip\r\n";
	else if (ext.compare(".tar") == 0)
		headers << "content-type: application/x-tar\r\n";
	else if (ext.compare(".mp4") == 0)
		headers << "content-type: video/mp4\r\n";
	else if (ext.compare(".mpeg") == 0)
		headers << "content-type: video/mpeg\r\n";
	else if (ext.compare(".avi") == 0)
		headers << "content-type: video/x-msvideo\r\n";
	else if (ext.compare(".avif") == 0)
		headers << "content-type: image/avif\r\n";
	else if (ext.compare(".ogg") == 0)
		headers << "content-type: audio/ogg\r\n";
	else if (ext.compare(".mp3") == 0)
		headers << "content-type: audio/mpeg\r\n";
	else if (ext.compare(".wav") == 0)
		headers << "content-type: audio/wav\r\n";
	else if (ext.compare(".mov") == 0)
		headers << "content-type: video/quicktime\r\n";
	else if (ext.compare(".ppt") == 0)
		headers << "content-type: application/vnd.ms-powerpoint\r\n";
	else if (ext.compare(".xls") == 0)
		headers << "content-type: application/vnd.ms-excel\r\n";
	else if (ext.compare(".doc") == 0)
		headers << "content-type: application/msword\r\n";
	else if (ext.compare(".csv") == 0)
		headers << "content-type: text/csv\r\n";
	else if (ext.compare(".txt") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".rtf") == 0)
		headers << "content-type: application/rtf\r\n";
	else if (ext.compare(".shtml") == 0)
		headers << "content-type: text/html\r\n";
	else if (ext.compare(".php") == 0)
		headers << "content-type: application/php\r\n";
	else if (ext.compare(".jsp") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".swf") == 0)
		headers << "content-type: application/x-shockwave-flash\r\n";
	else if (ext.compare(".ttf") == 0)
		headers << "content-type: application/x-font-truetype\r\n";
	else if (ext.compare(".eot") == 0)
		headers << "content-type: application/vnd.ms-fontobject\r\n";
	else if (ext.compare(".woff") == 0)
		headers << "content-type: application/font-woff\r\n";
	else if (ext.compare(".woff2") == 0)
		headers << "content-type: font/woff2\r\n";
	else if (ext.compare(".ics") == 0)
		headers << "content-type: text/calendar\r\n";
	else if (ext.compare(".vcf") == 0)
		headers << "content-type: text/x-vcard\r\n";
	else if (ext.compare(".mid") == 0)
		headers << "content-type: audio/midi\r\n";
	else if (ext.compare(".midi") == 0)
		headers << "content-type: audio/midi\r\n";
	else if (ext.compare(".wmv") == 0)
		headers << "content-type: video/x-ms-wmv\r\n";
	else if (ext.compare(".webm") == 0)
		headers << "content-type: video/webm\r\n";
	else if (ext.compare(".3gp") == 0)
		headers << "content-type: video/3gpp\r\n";
	else if (ext.compare(".3g2") == 0)
		headers << "content-type: video/3gpp2\r\n";
	else if (ext.compare(".pl") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".py") == 0)
		headers << "content-type: text/x-python\r\n";
	else if (ext.compare(".java") == 0)
		headers << "content-type: text/x-java-source\r\n";
	else if (ext.compare(".c") == 0)
		headers << "content-type: text/x-c\r\n";
	else if (ext.compare(".cpp") == 0)
		headers << "content-type: text/x-c++\r\n";
	else if (ext.compare(".cs") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".rb") == 0)
		headers << "content-type: text/x-ruby\r\n";
	else if (ext.compare(".htm") == 0)
		headers << "content-type: text/html\r\n";
	else if (ext.compare(".shtml") == 0)
		headers << "content-type: text/html\r\n";
	else if (ext.compare(".xhtml") == 0)
		headers << "content-type: application/xhtml+xml\r\n";
	else if (ext.compare(".m4a") == 0)
		headers << "content-type: audio/mp4\r\n";
	else if (ext.compare(".mp4a") == 0)
		headers << "content-type: audio/mp4\r\n";
	else if (ext.compare(".oga") == 0)
		headers << "content-type: audio/ogg\r\n";
	else if (ext.compare(".ogv") == 0)
		headers << "content-type: video/ogg\r\n";
	else if (ext.compare(".ogx") == 0)
		headers << "content-type: application/ogg\r\n";
	else if (ext.compare(".oga") == 0)
		headers << "content-type: audio/ogg\r\n";
	else if (ext.compare(".m3u8") == 0)
		headers << "content-type: application/vnd.apple.mpegurl\r\n";
	else if (ext.compare(".qt") == 0)
		headers << "content-type: video/quicktime\r\n";
	else if (ext.compare(".ts") == 0)
		headers << "content-type: video/mp2t\r\n";
	else if (ext.compare(".xl") == 0)
		headers << "content-type: application/excel\r\n";
	else if (ext.compare(".cab") == 0)
		headers << "content-type: application/vnd.ms-cab-compressed\r\n";
	else if (ext.compare(".msi") == 0)
		headers << "content-type: application/x-msdownload\r\n";
	else if (ext.compare(".dmg") == 0)
		headers << "content-type: application/x-apple-diskimage\r\n";
	else if (ext.compare(".exe") == 0)
		headers << "content-type: application/octet-stream\r\n";
	else if (ext.compare(".bin") == 0)
		headers << "content-type: application/octet-stream\r\n";
	else if (ext.compare(".ps") == 0)
		headers << "content-type: application/postscript\r\n";
	else if (ext.compare(".so") == 0)
		headers << "content-type: application/octet-stream\r\n";
	else if (ext.compare(".dll") == 0)
		headers << "content-type: application/octet-stream\r\n";
	else if (ext.compare(".m4v") == 0)
		headers << "content-type: video/x-m4v\r\n";
	else if (ext.compare(".ser") == 0)
		headers << "content-type: application/java-serialized-object\r\n";
	else if (ext.compare(".sh") == 0)
		headers << "content-type: application/x-sh\r\n";
	else if (ext.compare(".log") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".diff") == 0)
		headers << "content-type: text/x-diff\r\n";
	else if (ext.compare(".patch") == 0)
		headers << "content-type: text/x-diff\r\n";
	else if (ext.compare(".xhtml") == 0)
		headers << "content-type: application/xhtml+xml\r\n";
	else if (ext.compare(".php") == 0)
		headers << "content-type: application/x-httpd-php\r\n";
	else if (ext.compare(".plist") == 0)
		headers << "content-type: application/xml\r\n";
	else if (ext.compare(".sln") == 0)
		headers << "content-type: text/plain\r\n";
	else if (ext.compare(".tiff") == 0)
		headers << "content-type: image/tiff\r\n";
	else if (ext.compare(".app") == 0)
		headers << "content-type: application/octet-stream\r\n";
	else if (ext.compare(".ics") == 0)
		headers << "content-type: text/calendar\r\n";
}


std::string		Client::getHttpMsg(int code)
{
	switch (code)
	{
		case 0: return "No error.";
		
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 103: return "Checkpoint";

		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi-Status";
		case 208: return "Already Reported";

		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Switch Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";

		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 418: return "I'm a teapot";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Request";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";

		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";

		default: return "Unknown Error";
	}
}
