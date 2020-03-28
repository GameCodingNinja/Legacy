<commandScripts>

  <command name="inactive" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
    </scriptStep>
  </command>

  <command name="active" endMode="loop">
    <scriptStep>
      <SetVisible visible="true"/>
    </scriptStep>
  </command>

  <command name="selected" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor r="0.15" g="0.29" b="0.46"/>
    </scriptStep>
  </command>
  
  <command name="disabled" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
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
