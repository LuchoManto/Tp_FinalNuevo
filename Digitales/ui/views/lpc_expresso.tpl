% rebase('base.tpl')

<!--Main div to insert in base-->
<div id="main_div_module" class="container main_container">
    <!-- Send specific serial div-->
    <div id="specific_serial" class="container padding_top30">
        <div class="col-xs-2">
          <label class="control-label">Enviar por serial: </label>
        </div>
        <div class="col-xs-6">
          <input id="send_serial_input" class="form-control" type="text" placeholder="data to send">
        </div>
        <div class="col-xs-2">
            <button id="send_serial_button" class="btn btn-primary">Send</button>
        </div>
    </div>
    <!-- Engine measurement value div -->
    <div id="engine_value" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">Medicion CE: </label>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-primary send_serial" value="a">CE</button>
        </div>
    </div>
    <!-- RPM measurement value div -->
    <div id="rpm_value" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">Valor RPM: </label>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-primary send_serial" value="r">RPM</button>
        </div>
    </div>
    <!-- Engine status div -->
    <div id="engine_status" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">Estado del motor: </label>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-success send_serial" value="1">Prender</button>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-danger send_serial" value="0">Apagar</button>
        </div>
    </div>
    <!-- Engine gain div -->
    <div id="engine_gain" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">Velocidad preestablecida: </label>
        </div>
        <div class="col-xs-4">
            <div class="btn-group">
                <button class="btn btn-default dropdown-toggle" type="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
                    Velocidad <span class="caret"></span>
                </button>
                <ul class="dropdown-menu">
                    <li><a class="send_serial" value="2" href="#">1</a></li>
                    <li><a class="send_serial" value="3" href="#">2</a></li>
                    <li><a class="send_serial" value="4" href="#">3</a></li>
                    <li><a class="send_serial" value="5" href="#">4</a></li>
                    <li><a class="send_serial" value="6" href="#">5</a></li>
                    <li><a class="send_serial" value="7" href="#">6</a></li>
                    <li><a class="send_serial" value="8" href="#">7</a></li>
                    <li><a class="send_serial" value="9" href="#">8</a></li>
                </ul>
            </div>
        </div>
    </div>
    <!-- Low power div-->
    <div id="low_power" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">Variador de velocidad: </label>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-success send_serial" value="i">Incrementar</button>
        </div>
        <div class="col-xs-2">
            <button type="button" class="btn btn-danger send_serial" value="d">Disminuir</button>
        </div>
    </div>
    <!-- COM SELECT -->
    <div id="com_select" class="container padding_top20">
        <div class="col-xs-2">
            <label class="control-label">COM disponibles: </label>
        </div>
        <div class="col-xs-2">
            <div class="btn-group">
                <select id="com_dropdown" class="form-control" name="com_dropdown">
                        <!--<option selected="" disabled="" hidden="" value=""></option>-->
                </select>
                <!--<button class="btn btn-default dropdown-toggle" type="button" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">-->
                    <!--COM <span class="caret"></span>-->
                <!--</button>-->
                <!--<ul id='com_dropdown' class="dropdown-menu">-->
                    <!--&lt;!&ndash;<li><a class="send_serial" value="COM0" href="#">COM0</a></li>&ndash;&gt;-->
                <!--</ul>-->
            </div>
        </div>
        <div class="col-xs-2">
            <button id='conectar_uart' type="button" class="btn btn-success">Conectar</button>
        </div>
        <div class="col-xs-2">
            <button id="actualizar_coms" type="button" class="btn btn-primary">Actualizar</button>
        </div>
    </div>
</div>

<!--actualizar la lista de coms-->
<script>
$("#actualizar_coms").click(function(e){
    e.preventDefault();
    $('#com_dropdown').empty();
    //Post with the button
    $.getJSON('/getcoms',
        function(data)
        {
            for (key in data){
            $('#com_dropdown').append('<option value="'+ data[key] +'">'+ data[key] +'</option>');
            }

            //$('#com_dropdown').append('<li><a value="' + data[key] + '" href="#">' + data[key] + '</a></li>');
            //$('#renglon_2').html(data['renglon2'])
        }
        )
        .fail(function(){
                console.log("Error actualizando la pantalla del gps");
              }
    );
});
</script>


<!--Conectar al COM-->
<script>
$("#conectar_uart").click(function(e){
    e.preventDefault();
    //Post with the button
    $.ajax({
        url: '/conectar_serial/' + $("#com_dropdown").val(),
        cache:false, type: 'POST'
    });
});
</script>

<!--Script when send a specific value-->
<script>
$("#send_serial_button").click(function(e){
    e.preventDefault();
    //Post with the button
    $.ajax({
        url: '/send_serial/' + $("#send_serial_input").val(),
        cache:false, type: 'POST'
    });
});
</script>

<!--Script to connect/disconnect UART-->
<script>
$(".uart_connection").click(function(e){
    e.preventDefault();
    //Post to connect
    $.ajax({
        url: '/uart_state/' + $(this).attr("value"),
        cache:false, type: 'POST'
    });
});
</script>

<!--Script to send value of button by serial-->
<script>
$(".send_serial").click(function(e){
    e.preventDefault();
    //Post to connect
    $.ajax({
        url: '/send_serial/' + $(this).attr("value"),
        cache:false, type: 'POST'
    });
});
</script>
