<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.30" g="0.52" b="0.77"/>
      <ColorTo method="time" time="500" r="0.15" g="0.29" b=".46"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" r="0.30" g="0.52" b="0.77"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.53" g="0.78" b="1"/>
    </scriptStep>

    <scriptStep>
      <SetHold hold="50"/>
    </scriptStep>

    <scriptStep>
      <SetColor r="0.1" g="0.21" b="0.33"/>
    </scriptStep>
  </command>
  
  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>


  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>
  
</commandScripts>
