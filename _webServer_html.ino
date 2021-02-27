/*
   WebServer HTML Layout
*/
void sendHTML(int code, String data) {
  return sendHTML(code, "", data);
}

void sendHTML(int code, String title, String data) {
  if (title != "") title = " | " + title;
  String sendHTML_html;
  sendHTML_html += "<!doctype html><html lang=\"en\" class=\"h-100\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><title>Clock ST" + title + "</title>";
  sendHTML_html += "<link href=\"https://getbootstrap.com/docs/5.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\"><meta name=\"theme-color\" content=\"#7952b3\">";
  sendHTML_html += "<style>main > .container {padding: 60px 15px 0;} .footer {background-color: #f5f5f5;} .footer > .container{padding-right:15px;padding-left:15px;}</style>";
  sendHTML_html += "<body class=\"d-flex flex-column h-100\">";
  sendHTML_html += html_header_menu();
  sendHTML_html += "<main class=\"flex-shrink-0\"><div class=\"container mt-5\">";
  sendHTML_html += data;
  sendHTML_html += "</div></main>";
  sendHTML_html += "<footer class=\"footer mt-auto py-3 bg-light\"><div class=\"container\"><span class=\"text-muted\">Создано <a href=\"mailto:ilyast91@gmail.com\">ilyast91@gmail.com</a></span></div></footer>";
  sendHTML_html += "<script src=\"https://getbootstrap.com/docs/5.0/dist/js/bootstrap.bundle.min.js\"></script>";
  sendHTML_html += "<script src=\"https://cdn.jsdelivr.net/gh/ilyast91/clock_st@master/display_form/display_form.js\"></script>";
  sendHTML_html += "</body></html>";
  server.send(code, "text/html", sendHTML_html);
}

String html_header_menu() {
  String html_header_menu = "<header><nav class=\"navbar navbar-expand-md navbar-dark fixed-top bg-dark\"><div class=\"container-fluid\">";
  html_header_menu += "<a class=\"navbar-brand\" href=\"\\\">Clock ST</a>";
  html_header_menu += "<button class=\"navbar-toggler\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#navbarCollapse\" aria-controls=\"navbarCollapse\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">";
  html_header_menu += "<span class=\"navbar-toggler-icon\"></span></button><div class=\"collapse navbar-collapse\" id=\"navbarCollapse\"><ul class=\"navbar-nav me-auto mb-2 mb-md-0\">";
  html_header_menu += "<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings_led\">LED</a></li>";
  html_header_menu += "<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings_rtc\">Часы</a></li>";
  html_header_menu += "<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings_sensors\">Сенсоры</a></li>";
  html_header_menu += "<li class=\"nav-item\"><a class=\"nav-link\" href=\"/settings_wifi\">WIFI</a></li>";
  html_header_menu += "</ul>";
  html_header_menu += "<form class=\"d-flex\"><a class=\"btn btn-outline-danger\" href=\"\\restart\">Перезагрузка</a></form>";
  html_header_menu += "</div></div></nav></header>";
  return html_header_menu;
}

String createFormTextElement(String id, String label_name, String value) {
  String form_element;
  form_element += "<label for=\"" + id + "\" class=\"form-label\">" + label_name + "</label>";
  form_element += "<input type=\"text\" class=\"form-control\" id=\"" + id + "\" name=\"" + id + "\" value=\"" + value + "\" >";
  return createFormElement(form_element);
}

String createFormElement(String element) {
  return "<div class=\"mb-3 col\">" + element + "</div>";
}

String createIntensitySelect(String id, String label_name, String selectedIndex) {
  String intensity;

  intensity += "<label for=\"" + id + "\" class=\"form-label\">" + label_name + "</label>";
  intensity += "<select id=\"" + id + "\" name=\"" + id + "\" class=\"form-select\">";

  for (int i = 0; i < 16; i++) {
    intensity += "<option value=\"" + String(i) + "\" ";
    intensity += (selectedIndex.toInt() == i ? "selected" : "");
    intensity += ">" + String(i) + "</option>";
  }

  intensity += "</select>";
  return createFormElement(intensity);
}

String createYesNoSelect(String id, String label_name, String selectedValue) {
  String result;
  result += "<label for=\"" + id + "\" class=\"form-label\">" + label_name + "</label>";
  result += "<select id=\"" + id + "\" name=\"" + id + "\" class=\"form-select\">";
  result += "<option value=\"true\" ";
  result += (selectedValue == "true" ? "selected" : "");
  result += ">Да</option>";
  result += "<option value=\"false\" ";
  result += (selectedValue == "true" ? "" : "selected");
  result += ">Нет</option>";
  result += "</select>";
  return createFormElement(result);
}
