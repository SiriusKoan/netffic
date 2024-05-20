<template>
    <div class="flex w-full">
        <div>
            <h2 class="text-center">TCP</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source Port" column2="Times" :data="tcpStats['srcPortCounter']" />
                <TwoColTable column1="Destination Port" column2="Times" :data="tcpStats['dstPortCounter']" />
            </div>
        </div>
        <div>
            <h2 class="text-center">UDP</h2>
            <div class="flex w-full gap-1 items-start">
                <TwoColTable column1="Source Port" column2="Times" :data="udpStats['srcPortCounter']" />
                <TwoColTable column1="Destination Port" column2="Times" :data="udpStats['dstPortCounter']" />
            </div>
        </div>
    </div>
</template>

<script>
import TwoColTable from '../components/TwoColTable.vue'
export default {
    name: 'L4View',
    data() {
        return {
            tcpStats: {},
            udpStats: {},
        }
    },
    components: {
        TwoColTable
    },
    mounted() {
        setInterval(() => {
            fetch('http://work.isk:8000/api/stats/tcp')
                .then(response => response.json())
                .then(data => {
                    this.tcpStats = data
                    this.tcpStats['srcPortCounter'] = Object.fromEntries(
                        Object.entries(this.tcpStats['srcPortCounter']).sort(([, a], [, b]) => b - a)
                    )
                    this.tcpStats['dstPortCounter'] = Object.fromEntries(
                        Object.entries(this.tcpStats['dstPortCounter']).sort(([, a], [, b]) => b - a)
                    )
                })
            fetch('http://work.isk:8000/api/stats/udp')
                .then(response => response.json())
                .then(data => {
                    this.udpStats = data
                    this.udpStats['srcPortCounter'] = Object.fromEntries(
                        Object.entries(this.udpStats['srcPortCounter']).sort(([, a], [, b]) => b - a)
                    )
                    this.udpStats['dstPortCounter'] = Object.fromEntries(
                        Object.entries(this.udpStats['dstPortCounter']).sort(([, a], [, b]) => b - a)
                    )
                })
        }, 1000)
    },
}
</script>
