<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetColor r="0.14" g="0.29" b=".45" resetDefault="true"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetColor r="0.16" g="0.32" b="0.50"/>
      <ColorTo method="time" time="500" r="0.1" g="0.21" b=".33"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" r="0.16" g="0.32" b="0.50"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.18" g="0.36" b="0.56"/>
    </scriptStep>

    <scriptStep>
      <SetHold hold="50"/>
    </scriptStep>

    <scriptStep>
      <SetColor r="0.14" g="0.29" b=".45"/>
    </scriptStep>
  </command>
  
  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>


  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetColor a="0"/>
      <ColorTo method="time" time="250" a="1"/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetColor a="1"/>
      <ColorTo method="time" time="250" a="0"/>
    </scriptStep>
  </command>
  
</commandScripts>
