<commandScripts>

  <command name="fadeIn" endMode="stop">
    <scriptStep>
      <SetColor a="0"/>
      <SetHold hold="200"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" a="1"/>
    </scriptStep>
  </command>

  <command name="fadeOut" endMode="stop">
    <scriptStep>
      <SetColor a="1"/>
      <ColorTo method="time" time="400" a="0"/>
    </scriptStep>

    <scriptStep>
      <SetHold hold="200"/>
    </scriptStep>
  </command>

</commandScripts>
