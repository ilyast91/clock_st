document.addEventListener('DOMContentLoaded', function () {
    updateForm();    
});

function updateForm() {
	if (document.getElementById('led_devices_form')) {
		document.getElementById('led_devices_form').innerHTML = '';
		var led_devices = JSON.parse(document.getElementById('led_devices').value) || [];
		for(var i = 0; i < led_devices.length; i++) {
			createDevice(led_devices[i], i);
		}
		document.getElementById('led_devices_form').append(generateButtonPanel('text-center', 'btn btn-success m-3', 'Добавить дисплей', 'onClickAddDisplay'));
	}
}

function onClickAddDisplay() {
    var led_devices = JSON.parse(document.getElementById('led_devices').value) || [];
    led_devices.push({});
    document.getElementById('led_devices').value = JSON.stringify(led_devices);
    updateForm()
}

function onClickRemoveDisplay(display_index) {
    var led_devices = JSON.parse(document.getElementById('led_devices').value) || [];
    led_devices.splice(display_index, 1);
    document.getElementById('led_devices').value = JSON.stringify(led_devices);
    updateForm()
}

function onClickAddSource(display_index, side_name) {
    var led_devices = JSON.parse(document.getElementById('led_devices').value) || [];
    if (!led_devices[display_index]['sources_' + side_name]) {
        led_devices[display_index]['sources_' + side_name] = []
    }
    led_devices[display_index]['sources_' + side_name].push({});
    document.getElementById('led_devices').value = JSON.stringify(led_devices);
    updateForm()
}

function onClickRemoveSource(display_index, side_name, source_index) {
    var led_devices = JSON.parse(document.getElementById('led_devices').value) || [];
    led_devices[display_index]['sources_' + side_name].splice(source_index, 1);
    document.getElementById('led_devices').value = JSON.stringify(led_devices);
    updateForm()
}

function onChangeSource() {
    saveToJson();
}

function saveToJson() {
    var newJson = [];
    var form = document.getElementById('led_devices_form');
    var devices = form.getElementsByClassName('json_device');

    for (device of devices) {
        var corr_intensity = device.getElementsByClassName('json_corr_intensity');
        var new_corr_intensity = corr_intensity[0].value+'' || '0';

        var new_sources_left = [];
        var sources_left = device.getElementsByClassName('json_sources_left');
        for(var i = 0; i < sources_left.length; i++) {
            var source = sources_left[i];
            var source_name = source.getElementsByClassName('json_source');
            var source_interval = source.getElementsByClassName('json_interval');
            new_sources_left.push({
                source: source_name[0].value,
                interval: source_interval[0].value,
            });            
        }

        var new_sources_right = [];
        var sources_right = device.getElementsByClassName('json_sources_right');
        for(var i = 0; i < sources_right.length; i++) {
            var source = sources_right[i];
            var source_name = source.getElementsByClassName('json_source');
            var source_interval = source.getElementsByClassName('json_interval');
            new_sources_right.push({
                source: source_name[0].value,
                interval: source_interval[0].value,
            });
        }

        newJson.push({
            corr_intensity: new_corr_intensity,
            sources_left: new_sources_left,
            sources_right: new_sources_right,
        });
    }
    document.getElementById('led_devices').value = JSON.stringify(newJson);
}

function createDevice(device, index) {
    var sources_left = device.sources_left || [];
    var sources_right = device.sources_right || [];
    var corr_intensity = device.corr_intensity || '0';
    var device_element = createElement('div', 'card m-3 json_device');
    var device_element_header = createElement('div', 'card-header', 'Дисплей с индексом: ' + index);   
    var device_element_body = createElement('div', 'card-body');

    // Corection Intensity input
    var intensity_div = createElement('div', 'card-text');
    intensity_div.append(generateInput('corr_intensity','Корректировка уровня яркости', 'json_corr_intensity', corr_intensity, onChangeSource));
    device_element_body.append(intensity_div);

    // Sources
    var row = createElement('div', 'row');
    
    var col_left = generateSourcesRows(sources_left, 'Левая сторона, сек', 'left', index);
    col_left.append(generateButtonPanel('d-grid', 'btn btn-primary', 'Добавить источник', 'onClickAddSource', index, 'left'));
    row.append(col_left);
    
    var col_right = generateSourcesRows(sources_right, 'Правая сторона, сек', 'right', index);
    col_right.append(generateButtonPanel('d-grid', 'btn btn-primary', 'Добавить источник', 'onClickAddSource', index, 'right'));
    row.append(col_right);

    device_element_body.append(row);
    
    device_element.append(device_element_header);
    device_element.append(device_element_body);
    device_element.append(generateButtonPanel('card-footer text-end','btn btn-danger btn-sm','Удалить дисплей', 'onClickRemoveDisplay', index));
    
    document.getElementById('led_devices_form').append(device_element);
}

function generateSourceSelect(name, value, onChange) {
    var sources = ['time', 'date', 'year', 'bmp_temp', 'bmp_pressure', 'bmp_altitude', 'bmp_humidity', 'aht10_temp', 'aht10_humidity'];
    for(var i=0;i<8;i++) sources.push('temp_'+i);

    var select = createElement('select', 'form-select json_source');
    select.setAttribute('name', name);
    select.append(createElement('option','', '---'));
    select.onchange = onChange;
    for(source of sources) {
        var option = createElement('option','', source);
        option.setAttribute('value', source);
        if (value == source) option.setAttribute('selected', '');
        select.append(option);
    }
    return select;
}

function generateInput(name, label, className, value, onChange) {
    var div = createElement('div', 'mb-3 col');

    var div_input = createElement('input', 'form-control ' + className);
    div_input.setAttribute('type', 'number');
    div_input.setAttribute('name', name);
    div_input.setAttribute('value', value || '');
    div_input.onchange = onChange;
    
    if (label) {
        div.append(createElement('label', 'form-label', label));
    }

    div.append(div_input);
    return div;
}

function generateSourcesRows(sourcesList, sideTitle, side, index) {
    var sources_row = createElement('div', 'col-md-6 col-sm-12 my-4');
    sources_row.append(createElement('h5','',sideTitle));
    for(var i = 0; i < sourcesList.length; i++) {
        var source_obj = sourcesList[i];
        var source_obj_row = createElement('div', 'row json_sources_' + side);

        var source_obj_col_left = createElement('div', 'col-5');
        source_obj_col_left.append(generateSourceSelect('source', source_obj.source, onChangeSource));

        var source_obj_col_right = createElement('div', 'col-5');
        source_obj_col_right.append(generateInput('interval', null, 'json_interval', source_obj.interval, onChangeSource));

        var btn = generateButtonPanel('col-2', 'btn btn-danger', '-', 'onClickRemoveSource', index, side, i); 

        source_obj_row.append(source_obj_col_left);
        source_obj_row.append(source_obj_col_right);
        source_obj_row.append(btn);
        sources_row.append(source_obj_row);
    }
    return sources_row;
}

function generateButtonPanel(pnl_class, btn_class, btn_text, onClickName, display_index, side_name, source_index) {
    var btn_pnl = createElement('div', pnl_class);
    var btn_pnl_btn_add = createElement('a', btn_class, btn_text);
    
    btn_pnl_btn_add.onclick = function (e) {
        e.preventDefault();
        switch(onClickName) {
            case 'onClickAddDisplay':
                btn_pnl_btn_add.onclick = onClickAddDisplay();
                break;
            case 'onClickRemoveDisplay':
                btn_pnl_btn_add.onclick = onClickRemoveDisplay(display_index);
                break;
            case 'onClickAddSource':
                btn_pnl_btn_add.onclick = onClickAddSource(display_index, side_name);
                break;
            case 'onClickRemoveSource':
                btn_pnl_btn_add.onclick = onClickRemoveSource(display_index, side_name, source_index);
                break;
        }
    }
    btn_pnl.append(btn_pnl_btn_add);
    return btn_pnl
}

function handleBtnEvent(e) {
    e.preventDefault();
}

function createElement(tag, className, html) {
    var el = document.createElement(tag);
    el.className = className;
    if (html) el.innerHTML = html;
    return el;
}