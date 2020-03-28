<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetVisible visible="false"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.1" g="0.21" b="0.33"/>
      <ColorTo method="time" time="500" r="0.04" g="0.08" b="0.12" />
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="500" r="0.1" g="0.21" b="0.33"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetColor r="0.15" g="0.29" b="0.46"/>
      <ColorTo method="time" time="120" r="0.04" g="0.08" b="0.12"/>
    </scriptStep>

    <scriptStep>
      <ColorTo method="time" time="120" r="0.15" g="0.29" b="0.46"/>
    </scriptStep>

    <scriptStep>
      <SetVisible visible="false"/>
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
