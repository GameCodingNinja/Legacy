<commandScripts>

  <command name="transitionIn" endMode="stop">
    <scriptStep>
      <SetVisible visible="true"/>
      <SetColor a="0"/>
      <ColorToDefault method="time" time="250" a=""/>
    </scriptStep>
  </command>

  <command name="transitionOut" endMode="stop">
    <scriptStep>
      <SetColorToDefault a=""/>
      <ColorTo method="time" time="250" a="0"/>
    </scriptStep>
   
  </command>

</commandScripts>
