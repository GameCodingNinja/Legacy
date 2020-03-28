<commandScripts>

  <command name="fadeIn" endMode="stop">
    <scriptStep>
      <SetColor a="0"/>
      <SetHold hold="200"/>
    </scriptStep>
    
    <scriptStep>
      <ColorToDefault method="time" time="500" a=""/>
    </scriptStep>
  </command>

  <command name="fadeOut" endMode="stop">
    <scriptStep>
      <SetColorToDefault a=""/>
      <ColorTo method="time" time="400" a="0"/>
    </scriptStep>
  </command>

</commandScripts>
